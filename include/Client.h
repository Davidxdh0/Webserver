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
    Client(int socket /*int port*/);
    ~Client();

    void                handleRequest();
    void                setResponse();
    void                writeResponse();
    void                configure();
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
//    int                 _port;
    clientState         _state;
    std::stringstream   _requestRaw;
	// size_t				_total_read;



    int                 readRequest();
};


#endif //WEBSERV_CLIENT_H
