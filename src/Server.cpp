//
// Created by Alle Janse on 6/6/23.
//

#include "Server.h"

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/fcntl.h>
#include "utils.h"
#include "Client.h"

Server::Server(const std::string& ip_address, Config& conf) :
_port(conf.getPort()), _socket(), _socketAddress(), _socketAddress_len(sizeof(_socketAddress)), _virtualhosts(conf.getHosts())
{
    _socketAddress.sin_family = AF_INET;
    _socketAddress.sin_port = htons(_port);

    if (createSocket() != 0)
    {
        std::cout << "Failed to start server with PORT: " << ntohs(_socketAddress.sin_port);
    }
}

Server::~Server() {
}

int Server::createSocket() {
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket < 0)
    {
        exitWithError("Cannot create socket");
        return 1;
    }
    int reuse = 1;
    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) == -1) {
        exitWithError("Error setting socket options");
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

void Server::closeServer() const
{
    close(_socket);
    exit(0);
}

void Server::startListen(int kqueuFd) const
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
    log("------ Server event registered in kqueu ------\n\n");
}

void Server::acceptConnection(int kqueu_fd)
{
    int opt_value = 1;
    setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt_value, sizeof(opt_value));

    int client_socket = accept(_socket, (sockaddr *)&_socketAddress, &_socketAddress_len);
    setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt_value, sizeof(opt_value));
    if (client_socket < 0)
    {
        std::ostringstream ss;
        ss << "Server failed to accept incoming connection from ADDRESS: " << inet_ntoa(_socketAddress.sin_addr) << "; PORT: " << ntohs(_socketAddress.sin_port);
        exitWithError(ss.str());
    }
    if (fcntl(client_socket, F_SETFL, O_NONBLOCK) < 0)
    {
        exitWithError("Cannot set socket to non-blocking");
    }
    createClient(kqueu_fd, client_socket);
}

void Server::createClient(int kqueu_fd, int client_socket) const
{
    Client* client = new Client(client_socket);
    struct kevent event[2];

    EV_SET(&event[0], client_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, client);
    EV_SET(&event[1], client_socket, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, client);
    kevent(kqueu_fd, event, 2, nullptr, 0, nullptr);
    log("------ Client event registered in kqueu ------\n\n");
}
