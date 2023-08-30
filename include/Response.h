//
// Created by Alle Janse on 6/13/23.
//

#ifndef WEBSERV_RESPONSE_H
#define WEBSERV_RESPONSE_H

#include <string>
#include <sstream>
#include "Path.h"
#include "Request.h"
#include "vector"
#include "utils.h"
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <cstdio>
#include <dirent.h>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <ctime>
#include<unistd.h>   

class Response {

public:
    Response();
    ~Response();

    void            setVersion(const std::string &version);
	void            setBody(const std::string &body);
    void            setStatusCode(const std::string &statusCode);
    void            setStatusMessage(const std::string &statusMessage);
	void            setHeaders(const std::string &headers);
    void            setContentLength();
	void            setContentType(const std::string &headers);
	std::string		getStatusCode(void);
	std::string		getStatusMessage(void);
	std::string		getBody();
	
    void            loadBody(const Path& path);
    void            loadCgi(const Path& path);
    void            setResponseString();
    std::string     getResponseString() { return _responseString; };

	bool 			RequestDirectory(void);
	void			directoryListing(std::string dirpath);
	void			showDir(void);
	void			createIndex(void);
	bool 			isDirectory(std::string path);
	bool			findFile(std::string file, std::string path);
	void 			setErrorPage(Path &obj);
	void			errorCodeMessage();
	void			setErrorCodeMessage(std::string code);
	void			deletePage(std::string path);
	void			upload();
	int 			uploadFile();
	bool			checkMethod(std::string &str);
	bool			isUpload();
	bool 			hasAccess(std::string filepath, std::fstream &filestr);
	

private:
	std::vector<std::pair<std::string, std::string> >	_errorMessage;
    Path            _path;
    std::string     _version;
    std::string     _statusCode;
    std::string     _statusMessage;
    std::string     _headers;
	std::string     _contentType;
	std::string     _contentLength;
    std::string     _body;
    std::string     _responseString;
	
};


#endif //WEBSERV_RESPONSE_H
