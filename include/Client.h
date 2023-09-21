//
// Created by Alle Janse on 6/13/23.
//

#ifndef WEBSERV_CLIENT_H
#define WEBSERV_CLIENT_H

#include "Request.h"
#include "Response.h"
#include "Path.h"
#include "Config.h"
#include "Settings.h"
#include "utils.h"
#include <sys/event.h>

class Client {
public:
    Client();
    Client(int socket, Settings* vhosts);
    ~Client();

    void                handleRequest(long data);
    void                writeResponse();
    void                setLocal(std::string &port);

    int                 getState() { return _state; };
	std::stringstream   &getRequestRaw();
	const Request		&getRequest() const;
	const Response		&getResponse() const;
	const Path			&getPath() const;
    std::string         getErrorPath();
    const int           &getSocket() const {return _socket; };
private:

    Request             _request;
    Response            _response;
    Path                _path;
    int                 _socket;
    clientState         _state;
    std::stringstream   _requestRaw;
	std::string         _chunked;
    Settings            _settings;
    Settings*           _vhosts;


    int                 readRequest(long data);
    void                configure();
    void                redirect();
    int                 readChunked(std::string bufferstr, int chunkedrequest);
    void                setResponse();
    void                checkMethod();
    void                index();
};


#endif //WEBSERV_CLIENT_H
