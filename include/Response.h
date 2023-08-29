//
// Created by Alle Janse on 6/13/23.
//

#ifndef WEBSERV_RESPONSE_H
#define WEBSERV_RESPONSE_H


#include <string>
#include <sstream>
#include "Path.h"
#include "Request.h"


class Response {

public:
    Response();
    ~Response();

    void            setVersion(const std::string &version);
    void            setStatusCode(const std::string &statusCode);
    void            setStatusMessage(const std::string &statusMessage);
    void            setHeaders(const std::string &headers);
	std::string		getStatusCode(void);
	std::string		getStatusMessage(void);
	
    void            loadBody(const Path& path);
    void            loadCgi(const Path& path);
    void            setResponseString();
    std::string     getResponseString() { return _responseString; };

	bool 			RequestDirectory(void);
	void			directoryListing(std::string dirpath);
	void			deletePage(std::string path);
	void			showDir(void);
	void			createIndex(void);
	bool 			isDirectory(std::string path);
	bool			findFile(std::string file, std::string path);
	void			upload(std::stringstream &requestRaw);
	void 			setErrorPage(Path &obj);
	bool			checkMethod(std::string &str);

private:
    Path            _path;
    std::string     _version;
    std::string     _statusCode;
    std::string     _statusMessage;
    std::string     _headers;
    std::string     _body;
    std::string     _responseString;
};


#endif //WEBSERV_RESPONSE_H
