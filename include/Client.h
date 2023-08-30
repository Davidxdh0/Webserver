//
// Created by Alle Janse on 6/13/23.
//

#ifndef WEBSERV_CLIENT_H
#define WEBSERV_CLIENT_H

#include "Request.h"
#include "Response.h"
#include "Path.h"
#include "utils.h"
#include <sys/event.h>

class Client {
public:
    Client();
    Client(int socket);
    ~Client();

    void                handleRequest();
    void                setResponse();
    void                writeResponse();
    int                 getState() { return _state; };
	std::stringstream   &getRequestRaw();
	const Request		&getRequest() const;
	const Response		&getResponse() const;
	const Path			&getPath() const;
	static Client		&getInstance() {static Client instance; return instance;}
private:

    Request             _request;
    Response            _response;
    Path                _path;
    int                 _socket;
    clientState         _state;
    std::stringstream   _requestRaw;



    int                 readRequest();
};


#endif //WEBSERV_CLIENT_H
