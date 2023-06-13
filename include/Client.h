//
// Created by Alle Janse on 6/13/23.
//

#ifndef WEBSERV_CLIENT_H
#define WEBSERV_CLIENT_H

#include "Request.h"
#include "Response.h"
#include <sys/event.h>

class Client {
public:
    Client();
    Client(int socket);
    ~Client();

    void                handleRequest();
    void                sendResponse();

private:

    Request             _request;
    Response            _response;
    int                 _socket;
    struct kevent       _evSet;

    int                 readRequest(std::stringstream &ss);

};


#endif //WEBSERV_CLIENT_H
