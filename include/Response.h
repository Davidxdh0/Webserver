//
// Created by Alle Janse on 6/13/23.
//

#ifndef WEBSERV_RESPONSE_H
#define WEBSERV_RESPONSE_H

#include <string>
#include <sstream>
#include <unistd.h>
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
	void            setHeaders(const Path& path);
    void            setContentLength();
	void            setContentType(const std::string &headers);
	void			setHeader(const std::string &header){ _headers = header; };

	std::string		getStatusCode(void);
	std::string		getStatusMessage(void);
	std::string		getBody();
	std::string		getContentType(void);
	std::string		GetFilename(std::string line);
	
    void            loadBody(const Path& path);
    int             loadCgi(const Path& path, const Request& request, const std::string& cgiPath);
    void            setResponseString();
    std::string     getResponseString() { return _responseString; };
    int             readCgi(int cgi_fd);

	bool 			RequestDirectory(void);
	void			directoryListing(std::string dirpath, std::string indexSettings);
	void			createIndex(void);
	bool 			isDirectory(std::string path);
	bool			findFile(std::string file, std::string path);
	void 			setErrorPage(std::string root, std::string errorPage);
	void			errorCodeMessage();
	void			setErrorCodeMessage(std::string code);
	void			deletePage(std::string path, Settings *settings);
	void			upload();
	int 			uploadFile(std::stringstream& raw, Settings* settings, const std::string &contentType);
	std::string		uniqueFileName(std::string path, std::string file);
	bool			checkMethod(std::string &str);
	bool			isUpload(std::string path);
	bool 			hasAccess(const std::string& filepath, std::fstream &filestr);
	void			MakeFiles(std::stringstream& raw, std::string path, Settings &settings, const std::string &contentType);
	

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
	std::string     _boundary;
	std::vector<std::string> _raw;

    void    parseCgiResponse();
};


#endif //WEBSERV_RESPONSE_H
