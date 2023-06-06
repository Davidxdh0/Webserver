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


class Server
{
public:
    Server(std::string ip_address, int port);
    ~Server();
    void startListen();

private:
    std::string m_ip_address;
    int m_port;
    int m_socket;
    int m_new_socket;
    struct sockaddr_in m_socketAddress;
    unsigned int m_socketAddress_len;
    std::string m_serverMessage;
    int startServer();
    void closeServer();
    void acceptConnection(int &new_socket);
    std::string buildResponse();
    void sendResponse();
};

#endif //WEBSERV_DEV_SERVER_H
