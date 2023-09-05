//
// Created by Alle Janse on 6/6/23.
//

#include <sys/event.h>
#include <sstream>
#include <unistd.h>
#include "ServerControl.h"
#include "utils.h"
#include "Client.h"
#include "Server.h"

ServerControl::ServerControl() : _kq_fd(kqueue()), _servers()
{
    if (_kq_fd == -1)
        exitWithError("Failed to create kqueue");
}

ServerControl::ServerControl(Config*  port_configs) : _kq_fd(kqueue()), _servers()
{
    if (_kq_fd == -1)
        exitWithError("Failed to create kqueue");
    for (size_t i = 0; port_configs[i].getPort() != 0; i++)
    {
        Server  tmp("0.0.0.0", port_configs[i]);
        log("Starting server on port: " + to_string(port_configs[i].getPort()));
        tmp.startListen(_kq_fd);
        _servers.push_back(tmp);
    }

    webservLoop();
}

ServerControl::~ServerControl()
{
    close(_kq_fd);
    for (size_t i = 0; i < _servers.size(); i++)
    {
        close(_servers[i].getSocket());
    }
}

Server* ServerControl::checkIdentIsServer(int ident)
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
    struct kevent events[2];

    while (1){
        EV_SET(&events[0], 0, 0, 0, 0, 0, 0);
        EV_SET(&events[1], 0, 0, 0, 0, 0, 0);
        kevent(_kq_fd, nullptr, 0, events, 2, &timeout);
        for (int i = 0; i < 2; i++) {
            if (events[i].flags & EV_ERROR) {
                exitWithError("Error in kevent");
            }
            if (events[i].ident != 0) {
                Server *tmp = checkIdentIsServer(events[i].ident);
                if (tmp != nullptr) {
                    tmp->acceptConnection(_kq_fd);
                } else {
                    Client *client = static_cast<Client *>(events[i].udata);
                    if (events[i].flags & EV_EOF && events->data == 0) {
                        delete client;
                    } else if (events[i].filter == EVFILT_READ && client->getState() == READING) {
                        client->handleRequest(events[i].data);
                        EV_SET(events, events[i].ident, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, client);
                        kevent(_kq_fd, events, 1, nullptr, 0, nullptr);
                    } else if (events[i].filter == EVFILT_WRITE && client->getState() == RESPONDING) {
                        client->writeResponse();
                        delete client;
                    }
                }
            }
        }
    }
}
