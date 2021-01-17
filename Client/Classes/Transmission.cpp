#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <iostream>
#include <string>
#include <vector>

#include "Transmission.h"
#include "MessageHeader.h"

bool Transporter::init() {
    if ((sock_fd_ = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        std::cerr << "socket() failed: " << errno << std::endl;
        return false;
    }
    
    return true;
}

bool Transporter::cnct(const std::string& ip, const int port) {
    if(inet_pton(AF_INET, ip.c_str(), &server_addr_.sin_addr) == -1) {
        std::cerr << "inet_pton() failed: " << errno << std::endl;
        return false;
    }
    server_addr_.sin_family = AF_INET;
    server_addr_.sin_port = htons(port);
    
    if(connect(sock_fd_, (struct sockaddr*)&server_addr_, sizeof(server_addr_)) == -1){
        std::cerr << "connect() failed: " << errno << std::endl;
        return false;
    }
    return true;
}

void Transporter::sendmsg(const Msg& msg) {
    int MsgH_sz = sizeof(msg.first);
    
    memcpy(send_buf_, (char*)&msg.first, MsgH_sz);
    memcpy(send_buf_ + MsgH_sz, msg.second.c_str(), msg.second.size());
    
    size_t bytes;
    if ((bytes = send(sock_fd_, send_buf_, MsgH_sz + msg.second.size(), 0)) == -1) {
        std::cerr << "send() failed: " << errno << std::endl;
        close(sock_fd_);
    }
}

void Transporter::recvmsg(Msgs& msgs) {
    size_t bytes;
    if ((bytes = recv(sock_fd_, recv_buf_, sizeof(recv_buf_), 0)) == -1) {
        std::cerr << "recv() failed: " << errno << std::endl;
        close(sock_fd_);
    }else {
        SeparatePackage(recv_buf_, bytes, msgs);
    }
}

void Transporter::SeparatePackage(const char* buf, const size_t bytes, Msgs& msgs)
{
    Msg msg;
    MsgH msg_h;
    
    int MsgH_sz = sizeof(msg_h);
    
    memcpy(t_buf_ + surplus_, buf, bytes);
    surplus_ += static_cast<int>(bytes);
    int i;
    for (i = 0; i < surplus_;) {
        if (surplus_ - i < MsgH_sz)
            break;
        
        memcpy(&msg_h, t_buf_ + i, MsgH_sz);
        msg.first.type = ntohs(msg_h.type);
        msg.first.length = ntohs(msg_h.length);
        
        i += MsgH_sz;
        
        if (surplus_ - i < msg.first.length) {
            i -= MsgH_sz;
            break;
        }else {
            msg.second.assign(t_buf_ + i, msg.first.length);
            i += msg.first.length;
            
            msgs.emplace_back(msg);
        }
        
    }
    
    if (i  > 0) {
        surplus_ -= i;
        memcpy(t_buf_, t_buf_ + i, surplus_);
    }
}
