//
// Created by Alle Janse on 6/6/23.
//

#ifndef WEBSERV_DEV_SERVER_H
#define WEBSERV_DEV_SERVER_H

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string>
#include <sys/event.h>


class Server
{
public:
    Server(std::string ip_address, int port);
    ~Server();
    void startListen(int kqueue_fd);
    void acceptConnection(int kqueu_fd);
    int getSocket() const { return _socket; }

private:
    std::string _ip_address;
    int _port;
    int _socket;
    int _new_socket;
    struct sockaddr_in _socketAddress;
    unsigned int _socketAddress_len;
    std::string _serverMessage;


    int         startServer();
    void        closeServer();
    std::string buildResponse();
    void        sendResponse();
    void        queuClient(int kqueu_fd);
};

#endif //WEBSERV_DEV_SERVER_H
