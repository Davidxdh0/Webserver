//
// Created by Alle Janse on 6/6/23.
//

#ifndef WEBSERV_DEV_SERVER_H
#define WEBSERV_DEV_SERVER_H

#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <string>
#include <sys/event.h>
#include "Config.h"
#include "Settings.h"

class Config;

class Server
{
public:
    Server(const std::string& ip_address, Config& conf);
    ~Server();


    void startListen(int kqueuFd) const;
    void acceptConnection(int kqueu_fd);
    int getSocket() const { return _socket; }

private:
    std::string         _ip_address;
    int                 _port;
    int                 _socket;
    struct sockaddr_in  _socketAddress;
    unsigned int        _socketAddress_len;
    Settings*           _virtualhosts;

    int         createSocket();
    void        closeServer() const;
    void        createClient(int kqueu_fd, int client_socket) const;
};

#endif //WEBSERV_DEV_SERVER_H
