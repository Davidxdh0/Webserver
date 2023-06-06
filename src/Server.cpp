//
// Created by Alle Janse on 6/6/23.
//

#include "Server.h"

#include <iostream>
#include <sstream>
#include <unistd.h>
#include "../include/utils.h"

//const int BUFFER_SIZE = 30720;

Server::Server(std::string ip_address, int port) : m_ip_address(ip_address), m_port(port), m_socket(),
                                                   m_socketAddress(), m_socketAddress_len(sizeof(m_socketAddress)),
                                                   m_serverMessage(buildResponse())
{
    m_socketAddress.sin_family = AF_INET;
    m_socketAddress.sin_port = htons(m_port);
    m_socketAddress.sin_addr.s_addr = inet_addr(m_ip_address.c_str());

    if (startServer() != 0)
    {
        std::ostringstream ss;
        ss << "Failed to start server with PORT: " << ntohs(m_socketAddress.sin_port);
        log(ss.str());
    }
}

Server::~Server() {
}

int Server::startServer() {
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket < 0)
    {
        exitWithError("Cannot create socket");
        return 1;
    }
    if (bind(m_socket, (sockaddr *)&m_socketAddress, m_socketAddress_len) < 0)
    {
        exitWithError("Cannot connect socket to address");
        return 1;
    }
    return 0;
}

void Server::closeServer()
{
    close(m_socket);
    close(m_new_socket);
    exit(0);
}

void Server::startListen()
{
    if (listen(m_socket, 20) < 0)
    {
        exitWithError("Socket listen failed");
    }

    std::ostringstream ss;
    ss << "\n*** Listening on ADDRESS: " << inet_ntoa(m_socketAddress.sin_addr) << " PORT: " << ntohs(m_socketAddress.sin_port) << " ***\n\n";
    log(ss.str());
}

void Server::acceptConnection(int &new_socket)
{
    new_socket = accept(m_socket, (sockaddr *)&m_socketAddress, &m_socketAddress_len);
    if (new_socket < 0)
    {
        std::ostringstream ss;
        ss << "Server failed to accept incoming connection from ADDRESS: " << inet_ntoa(m_socketAddress.sin_addr) << "; PORT: " << ntohs(m_socketAddress.sin_port);
        exitWithError(ss.str());
    }
}

std::string Server::buildResponse()
{
    std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
    std::ostringstream ss;
    ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
       << htmlFile;

    return ss.str();
}

void Server::sendResponse()
{
    unsigned long bytesSent;

    bytesSent = write(m_new_socket, m_serverMessage.c_str(), m_serverMessage.size());

    if (bytesSent == m_serverMessage.size())
    {
        log("------ Server Response sent to client ------\n\n");
    }
    else
    {
        log("Error sending response to client");
    }
}