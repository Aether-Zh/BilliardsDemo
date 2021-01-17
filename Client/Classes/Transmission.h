#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include <netinet/in.h>

#include <vector>
#include <string>

#include "MessageHeader.h"
#include "balls_info.pb.h"

const int MAX_BUFF = 65535;
const int MAX_MSG_LEN = 4096;
const std::string IP = "127.0.0.1";
const int PORT = 4316;

static class Transporter {
public:
    Transporter() = default;
    ~Transporter() = default;
    
    Transporter(const Transporter&) = delete;
    Transporter(Transporter&&) = delete;
    
    bool init();
    bool cnct(const std::string& ip, const int port);
    void sendmsg(const Msg& msg);
    void recvmsg(Msgs& msgs);
private:
    void SeparatePackage(const char* buf, const size_t bytes, Msgs& msgs);
    
    int sock_fd_;
    char recv_buf_[MAX_BUFF];
    char send_buf_[MAX_BUFF];
    char t_buf_[MAX_BUFF + MAX_MSG_LEN];
    int surplus_ = 0;
    struct sockaddr_in server_addr_;
    
} transporter;

#endif /* TRANSMISSION_H */
