#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#include <thread>

#include "Server.h"
#include "balls_info.pb.h"
#include "MessageHeader.h"
#include "BallsMove.h"

int main(int argc, char* argv[]) {
    listener_ = CreateListener();
    if (listener_ == -1)
        return -1;
    
    int kq = kqueue();
    if (!Register(kq, listener_)) {
        std::cerr << "Register listener to kq failed.\n";
        return -1;
    }
    
    WaitEvent(kq);
    
    return 0;
}

int CreateListener() {
    int sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1) {
        std::cerr << "socket() failed:" << errno << std::endl;
        return -1;
    }
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(IP.c_str());
    if (bind(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr)) == -1) {
        std::cerr << "bind() failed:" << errno << std::endl;
        return -1;
    }
    
    if (listen(sock, 5) == -1) {
        std::cerr << "listen() failed:" << errno << std::endl;
        return -1;
    }
    
    return sock;
}

bool Register(int kq, int fd) {
    struct kevent changes;
    EV_SET(&changes, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    
    int ret = kevent(kq, &changes, 1, NULL, 0, NULL);
    if (ret == -1)
        return false;
    
    return true;
}

void WaitEvent(int kq) {
    struct kevent events[MAX_EVENT_COUNT];
    while (true) {
        int ret = kevent(kq, NULL, 0, events, MAX_EVENT_COUNT, NULL);
        if (ret == -1) {
            std::cerr << "kevent failed!" << errno << '\n';
            continue;
        }
        
        HandleEvent(kq, events, ret);
    }
}

void HandleEvent(int kq, struct kevent* events, int nevents) {
    for (int i = 0; i < nevents; i++) {
        int sock = static_cast<int>(events[i].ident);
        int data = static_cast<int>(events[i].data);
        int filter = events[i].filter;
        if (filter == EVFILT_READ) {
            if (sock == listener_) {
                Accept(kq, data);
            }else {
                Receive(sock, data);
            }
        }
    }
}

void Accept(int kq, int connSize) {
    for (int i = 0; i < connSize; i++) {
        int client = accept(listener_, NULL, NULL);
        if (client == -1) {
            std::cerr << "Accept failed.\n";
            continue;
        }
        
        if (!Register(kq, client)) {
            std::cerr << "Register client failed.\n";
            return;
        }else {
            std::thread(Send, client, 0).detach();
        }
    }
}

void Receive(int sock, int availBytes) {
    puts("recving");
    
    size_t bytes = recv(sock, recv_buf_, availBytes, 0);
    if (bytes == 0 || bytes == -1) {
        close(sock);
        std::cerr << "client close or recv failed: " << errno << std::endl;
        return;
    }else {
        std::cerr << "recv() succeeded: " << bytes << std::endl;
        
        // Write buf to the receive queue.
        SeparatePackage(sock, recv_buf_, bytes, HandleMsg);
    }
}

void Send(int sock, int availBytes) {
    BallsInfo balls_info;
    BallInfo* pball_info;
    Msg msg;
    size_t bytes, MsgH_sz = sizeof(msg.first);
    std::vector<Ball*> balls;
    
    RemoveBall rm_ball_info;
    for (;;) {
        if (rm_sock != -1) {
            rm_ball_info.set_sock(rm_sock);
            rm_sock = -1;
            
            msg.first.type = htons(RM_CMD_TYPE);
            msg.second = rm_ball_info.SerializeAsString();
            msg.first.length = htons(msg.second.size());
        }else {
            balls = balls_world.get_balls();
            if (balls.size() == 0)
                continue;
            
            for (int i = 0; i < balls.size(); ++i) {
                pball_info = balls_info.add_ball();
                pball_info->set_sock(balls[i]->sock_);
                pball_info->set_id(balls[i]->id_);
                pball_info->set_r(balls[i]->r_);
                pball_info->set_x(balls[i]->x_);
                pball_info->set_y(balls[i]->y_);
            }
            
            msg.first.type = htons(DATA_TYPE);
            msg.second = balls_info.SerializeAsString();
            msg.first.length = htons(msg.second.size());
            
            balls_info.Clear();
        }
        memcpy(send_buf_, (char*)&msg.first, MsgH_sz);
        memcpy(send_buf_ + MsgH_sz, msg.second.c_str(), msg.second.size());
        
        if ((bytes = send(sock, send_buf_, MsgH_sz + msg.second.size(), 0)) == -1) {
            while (rm_sock != -1);
            rm_sock = sock;
            std::cerr << "send() failed: " << errno << '\n';
            close(sock);
            for (auto& i : sock_ids[sock]) {
                balls_world.rm_ball(sock, i);
            }
            sock_ids.erase(sock);
            break;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(1000000 / TPS));      // TPS
    }
}

void SeparatePackage(int sock, const char* buf, size_t bytes, HandleMsg_t call_back)
{
    Msg msg;
    MsgH msg_h;
    
    int MsgH_sz = sizeof(msg_h);
    
    memcpy(t_buf_ + surplus, buf, bytes);
    surplus += bytes;
    int i;
    for (i = 0; i < surplus;) {
        if (surplus - i < MsgH_sz)
            break;
        
        memcpy(&msg_h, t_buf_ + i, MsgH_sz);
        msg.first.type = ntohs(msg_h.type);
        msg.first.length = ntohs(msg_h.length);
        
        i += MsgH_sz;
        
        if (surplus - i < msg.first.length) {
            i -= MsgH_sz;
            break;
        }else {
            msg.second.assign(t_buf_ + i, msg.first.length);
            i += msg.first.length;
            
            call_back(sock, msg.first.type, msg.second);
        }
    }
    
    if (i  > 0) {
        surplus -= i;
        memcpy(t_buf_, t_buf_ + i, surplus);
    }
}

void HandleMsg(int sock, const unsigned short type, const std::string& msg) {
    if (type == CREATE_CMD_TYPE) {
        CreateBall create_ball_info;
        create_ball_info.ParseFromString(msg);
        Ball* pb = new Ball(sock, create_ball_info.id(), create_ball_info.x(), create_ball_info.y());
        balls_world.add_ball(pb);
        sock_ids[pb->sock_].emplace_back(pb->id_);
        pb = nullptr;
    }else if (type == MOVE_CMD_TYPE) {
        MoveBall move_ball_info;
        move_ball_info.ParseFromString(msg);
        for (auto& i : balls_world.get_balls()) {
            if (i->sock_ == move_ball_info.sock() && i->id_ == move_ball_info.id()) {
                i->v_ = std::make_pair(move_ball_info.v_val(), move_ball_info.v_dir());
                break;
            }
        }
    }
}
