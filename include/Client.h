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
#include <sys/time.h>

class Client {
public:
    Client();
    Client(int socket, Settings* vhosts, int kq_fd);
    ~Client();

    void                handleRequest(long data);
    void                writeResponse();
    void                setLocal(std::string &port);
    void                handleCgi(int cgi_fd);
	
	
    int                 getState() { return _state; };
	std::stringstream   &getRequestRaw();
	const Request		&getRequest() const;
	const Response		&getResponse() const;
	const Path			&getPath() const;
    const int           &getSocket() const { return _socket; };
    std::string         getErrorPath();
	int					getChunked() {return _chunked;}
    void                register_cgi_pipe();

private:
    int                 _kq_fd;
    Request             _request;
    Response            _response;
    Path                _path;
    int                 _socket;
    clientState         _state;
    std::stringstream   _requestRaw;
    Settings            _settings;
    Settings*           _vhosts;
    size_t				_dataSent;
    int					_chunked;

    int                 _cgi_pipe;
    int                 readRequest(long data);
    void                configure();
    void                redirect();
    int                 readChunked(std::string bufferstr, int chunkedrequest);
    void                setResponse();
    void                checkMethod();
    void                index();
    void                register_cgi_pid();
};


#endif //WEBSERV_CLIENT_H
