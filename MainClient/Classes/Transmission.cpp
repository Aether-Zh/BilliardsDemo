//
//  Transmission.cpp
//  ShengJi
//
//  Created by duole_083 on 2020/7/15.
//
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <iostream>

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

void Transporter::sendmsg(BallsInfo balls) {
    std::string msg = balls.SerializeAsString();
    
    MsgH message_header;
    message_header.type = htons(DATA_T);
    message_header.length = htons(msg.size());
    
    memcpy(buf_, (char*)&message_header, sizeof(message_header));
    memcpy(buf_ + sizeof(message_header), msg.c_str(), msg.size());
    
    int ret;
    if ((ret = send(sock_fd_, buf_, sizeof(message_header) + msg.size(), 0)) == -1) {
        std::cerr << "send() failed: " << errno << std::endl;
    } else {
        std::cout << "send() succeeded: " << ret << std::endl;
    }
}
