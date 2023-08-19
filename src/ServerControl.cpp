//
// Created by Alle Janse on 6/6/23.
//

#include <sys/event.h>
#include <sstream>
#include <unistd.h>
#include "../include/ServerControl.h"
#include "../include/utils.h"
#include "Client.h"

ServerControl::ServerControl() : _kq_fd(kqueue()), _servers()
{
    if (_kq_fd == -1)
        exitWithError("Failed to create kqueue");
}

ServerControl::ServerControl(vector<Config> configs) : _kq_fd(kqueue()), _servers()
{
    size_t size = configs.size();
    if (_kq_fd == -1)
        exitWithError("Failed to create kqueue");
    for (size_t i = 0; i < size; i++)
    {
        log("Starting server on port: " + to_string(configs[i].getPort()));
        ServerBlock  tmp("0.0.0.0", configs[i].getPort());
        tmp.startListen(_kq_fd);
        _servers.push_back(tmp);
    }

    webservLoop();
}

ServerControl::~ServerControl()
{
    close(_kq_fd);
}

ServerBlock* ServerControl::checkIdentIsServer(int ident)
{
    for (size_t i = 0; i < _servers.size(); i++)
    {
        if (_servers[i].getSocket() == ident)
            return &_servers[i];
    }
    return nullptr;
}

void   ServerControl::webservLoop() {
    struct timespec timeout = {};
    struct kevent events[1];

    while (1){
        EV_SET(events, 0, 0, 0, 0, 0, 0);
        kevent(_kq_fd, nullptr, 0, events, 1, &timeout);
        ServerBlock *tmp = checkIdentIsServer(events->ident);
        if (tmp != nullptr) {
            tmp->acceptConnection(_kq_fd);
        }
        else {
            Client *client = static_cast<Client *>(events->udata);
            if (events->filter == EVFILT_READ && client->getState() == READING) {
                client->handleRequest();
            } else if (events->filter == EVFILT_WRITE && client->getState() == RESPONDING) {
                client->writeResponse();
                delete client;
            }
        }
    }
}
