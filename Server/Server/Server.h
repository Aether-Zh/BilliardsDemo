#ifndef SERVER_H
#define SERVER_H

#include <sys/event.h>

#include <mutex>
#include <string>
#include <queue>
#include <map>
#include <atomic>

#include "balls_info.pb.h"

std::map<int, std::vector<int>> sock_ids;
std::atomic_int rm_sock(-1);

const std::string IP = "127.0.0.1";
const int PORT = 4316;

const int kReadEvent = 1;
const int kWriteEvent = 2;

const int MAX_EVENT_COUNT = 5000;
const int MAX_BUFF = 65535;
const int MAX_MSG_LEN = 4096;

int listener_;
char send_buf_[MAX_BUFF];
char recv_buf_[MAX_BUFF];
char t_buf_[MAX_BUFF + MAX_MSG_LEN];
int surplus = 0;

int CreateListener();
bool Register(int kq, int fd);
void WaitEvent(int kq);
void HandleEvent(int kq, struct kevent* events, int nevents);
void updateEvents(int efd, int fd, int events, bool modify);
void Accept(int kq, int connSize);
void Receive(int sock, int availBytes);
void Send(int sock, int availBytes);

using HandleMsg_t = void(int, const unsigned short, const std::string&);
void SeparatePackage(int sock, const char* buf, size_t bytes, HandleMsg_t call_back);

void HandleMsg(int sock, const unsigned short type, const std::string& msg);
#endif /* SERVER_H */
