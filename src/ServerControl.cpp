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

ServerControl::ServerControl(std::vector<pair<int, Settings* > >  port_configs) : _kq_fd(kqueue()), _servers()
{
    if (_kq_fd == -1)
        exitWithError("Failed to create kqueue");

    loadServers(port_configs);
    webservLoop();
}

void ServerControl::loadServers(std::vector<pair<int, Settings* > > port_configs) {
    for (size_t i = 0; i < port_configs.size() != 0; i++)
    {
        Config config_vec(port_configs[i].first);
        config_vec.setHosts(port_configs[i].second);
        Server  tmp(config_vec);
        log("Starting server on port: " + to_string(port_configs[i].first));
        tmp.startListen(_kq_fd);
        _servers.push_back(tmp);
    }
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
    struct kevent events[1];
	timeout.tv_sec = 0;      
	timeout.tv_nsec = 0;  
	
    while (1){
        EV_SET(&events[0], 0, 0, 0, 0, 0, 0);
        kevent(_kq_fd, nullptr, 0, events, 1, &timeout);
        if (events->flags & EV_ERROR)
            exitWithError("Error in kevent");
        if (events->ident != 0) {
            Server *tmp = checkIdentIsServer(events->ident);
            if (tmp != nullptr) {
                tmp->acceptConnection(_kq_fd);
            }else {
                Client *client = static_cast<Client *>(events->udata);
				// std::cout << "temp == NULL -> events filter: " << events->filter << " state:" << client->getState() << std::endl;
                if (events->flags & EV_EOF && events->data == 0 && static_cast<uintptr_t>(client->getSocket()) == events->ident){
                    delete client;
                } else if (events->filter == EVFILT_READ && client->getState() == READING) {
                    client->handleRequest(events->data);
//                    kevent(_kq_fd, events, 1, nullptr, 0, nullptr);
                } else if (events->filter == EVFILT_READ && client->getState() == CGIWAIT) {
                    client->handleCgi(events->ident);
                } else if (events->filter == EVFILT_WRITE && client->getState() == RESPONDING) {
                    client->writeResponse();
                    delete client;
                }
            }
        }
    }
}