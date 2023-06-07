//
// Created by Alle Janse on 6/6/23.
//

#include <sys/event.h>
#include <unistd.h>
#include "../include/ServerControl.h"
#include "../include/utils.h"

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
        log("Starting server on port: " + to_string(i));
        Server  tmp("0.0.0.0", configs[i].getPort());
        tmp.startListen(_kq_fd);
        _servers.push_back(tmp);
    }

    while (1){
        kevent(_kq_fd, NULL, 0, &_events, 1, NULL);
        if (checkIdentIsServer(_events.ident))
        {
            Server *tmp = static_cast<Server*>(_events.udata);
            tmp->acceptConnection(_kq_fd);
        }
        else if (_events.filter == EVFILT_READ){
            char buffer[1024];
            read(_events.ident, &buffer, sizeof(buffer));
            std::cout << buffer << std::endl;
        }
        else if (_events.filter == EVFILT_WRITE){
            write(_events.ident, "Hello World", 11);
        }
    }
}

ServerControl::~ServerControl()
{
    close(_kq_fd);
}

int ServerControl::checkIdentIsServer(int ident)
{
    for (size_t i = 0; i < _servers.size(); i++)
    {
        if (_servers[i].getSocket() == ident)
            return 1;
    }
    return 0;
}
