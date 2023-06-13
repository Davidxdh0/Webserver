//
// Created by Alle Janse on 6/6/23.
//

#include <sys/event.h>
#include <sstream>
#include <unistd.h>
#include "../include/ServerControl.h"
#include "../include/utils.h"

ServerControl::ServerControl() : _kq_fd(kqueue()), _servers(), _events()
{
    if (_kq_fd == -1)
        exitWithError("Failed to create kqueue");
}

ServerControl::ServerControl(vector<Config> configs) : _kq_fd(kqueue()), _servers(), _events()
{
    size_t size = configs.size();
    if (_kq_fd == -1)
        exitWithError("Failed to create kqueue");
    for (size_t i = 0; i < size; i++)
    {
        log("Starting server on port: " + to_string(i));
        ServerBlock  tmp("0.0.0.0", configs[i].getPort());
        tmp.startListen(_kq_fd);
        _servers.push_back(tmp);
    }

    struct timespec timeout = {};
    while (1){
        kevent(_kq_fd, nullptr, 0, &_events, 2, &timeout);
        ServerBlock *tmp = checkIdentIsServer(_events.ident);
        if (tmp != nullptr)
        {
            tmp->acceptConnection(_kq_fd);
        }
        else if (_events.filter == EVFILT_READ){
            char buffer[24];
            buffer[recv(_events.ident, &buffer, sizeof(buffer), 0)] = '\0';
            std::cout << buffer << std::endl;
        }
        else if (_events.filter == EVFILT_WRITE){
            std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your ServerBlock :) </p></body></html>";
            std::ostringstream ss;
            ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
               << htmlFile;

            write(_events.ident, ss.str().c_str(), ss.str().size());
            close(_events.ident);
        }
    }
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
