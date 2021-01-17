//
//  Transmission.h
//  ShengJi
//
//  Created by duole_083 on 2020/7/15.
//

#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include <netinet/in.h>

#include <string>

#include "balls_info.pb.h"

const int MAX_SEND_BUFF = 65535;
const std::string IP = "127.0.0.1";
const int PORT = 4313;

static class Transporter {
public:
    Transporter() = default;
    ~Transporter() = default;
    
    Transporter(const Transporter&) = delete;
    Transporter(Transporter&&) = delete;
    
    bool init();
    bool cnct(const std::string& ip, const int port);
    void sendmsg(BallsInfo balls);
    
private:
    int sock_fd_;
    char buf_[MAX_SEND_BUFF + 1];
    struct sockaddr_in server_addr_;
    
} transporter;

#endif /* TRANSMISSION_H */
