//
// Created by Alle Janse on 6/6/23.
//

#include "ServerBlock.h"

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/fcntl.h>
#include "../include/utils.h"
#include "Client.h"

ServerBlock::ServerBlock(const std::string& ip_address, int port) :
_ip_address(ip_address), _port(port), _socket(), _new_socket(),
_socketAddress(), _socketAddress_len(sizeof(_socketAddress)), _serverMessage(buildResponse())
{
    _socketAddress.sin_family = AF_INET;
    _socketAddress.sin_port = htons(_port);
    _socketAddress.sin_addr.s_addr = inet_addr(_ip_address.c_str());

    if (createSocket() != 0)
    {
        std::cout << "Failed to start server with PORT: " << ntohs(_socketAddress.sin_port);
    }
}

ServerBlock::~ServerBlock() {
}

int ServerBlock::createSocket() {
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket < 0)
    {
        exitWithError("Cannot create socket");
        return 1;
    }
    if (bind(_socket, (sockaddr *)&_socketAddress, _socketAddress_len) < 0)
    {
        exitWithError("Cannot connect socket to address");
        return 1;
    }
    if (fcntl(_socket, F_SETFL, O_NONBLOCK) < 0)
    {
        exitWithError("Cannot set socket to non-blocking");
        return 1;
    }
    return 0;
}

void ServerBlock::closeServer() const
{
    close(_socket);
    exit(0);
}

void ServerBlock::startListen(int kqueuFd) const
{
    if (listen(_socket, 20) < 0)
    {
        exitWithError("Socket listen failed");
    }

    std::ostringstream ss;
    ss << "\n*** Listening on ADDRESS: " << inet_ntoa(_socketAddress.sin_addr) << " PORT: " << ntohs(_socketAddress.sin_port) << " ***\n\n";
    log(ss.str());

    struct kevent event = {};

    EV_SET(&event, _socket, EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, nullptr);
    kevent(kqueuFd, &event, 1, nullptr, 0, nullptr);
    log("------ ServerBlock event registered in kqueu ------\n\n");
}

void ServerBlock::acceptConnection(int kqueu_fd)
{
    int client_socket = accept(_socket, (sockaddr *)&_socketAddress, &_socketAddress_len);

    if (client_socket < 0)
    {
        std::ostringstream ss;
        ss << "ServerBlock failed to accept incoming connection from ADDRESS: " << inet_ntoa(_socketAddress.sin_addr) << "; PORT: " << ntohs(_socketAddress.sin_port);
        exitWithError(ss.str());
    }
    if (fcntl(client_socket, F_SETFL, O_NONBLOCK) < 0)
    {
        exitWithError("Cannot set socket to non-blocking");
    }
    createClient(kqueu_fd, client_socket);
}

void ServerBlock::createClient(int kqueu_fd, int client_socket) const
{
    Client* client = new Client(client_socket);
    struct kevent event[2];

    EV_SET(&event[0], client_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, client);
    EV_SET(&event[1], client_socket, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, client);
    kevent(kqueu_fd, event, 2, nullptr, 0, nullptr);
    log("------ Client event registered in kqueu ------\n\n");
}

std::string ServerBlock::buildResponse()
{
    std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your ServerBlock :) </p></body></html>";
    std::ostringstream ss;
    ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
       << htmlFile;

    return ss.str();
}

void ServerBlock::sendResponse()
{
    unsigned long bytesSent;

    bytesSent = write(_new_socket, _serverMessage.c_str(), _serverMessage.size());

    if (bytesSent == _serverMessage.size())
    {
        log("------ ServerBlock Response sent to client ------\n\n");
    }
    else
    {
        log("Error sending response to client");
    }
}