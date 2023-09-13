//
// Created by Alle Janse on 6/13/23.
//
#include "Response.h"
#include "Client.h" 
#include "utils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <cstdio>
#include <dirent.h>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <ctime>
#include <vector>
#include "MimeTypes.h"

Response::Response() {}

Response::~Response() {}

void	Response::setVersion(const std::string &version) {_version = version;}
void    Response::setBody(const std::string &body) { _body = body;}
void	Response::setStatusCode(const std::string &statusCode) {_statusCode = statusCode;}
void	Response::setStatusMessage(const std::string &statusMessage) {_statusMessage = statusMessage;}
void    Response::setContentLength(){_contentLength = std::string("Content-Length: " + std::to_string(_body.length() ));}
void    Response::setContentType(const std::string &type){_contentType = "Content-Type: " + type;}

std::string	Response::getStatusCode(void) {return _statusCode;}
std::string	Response::getStatusMessage(void) {return _statusMessage;}
std::string	Response::getBody(){return _body;}
std::string	Response::getContentType(void) { return _contentType;}

void Response::loadBody(const Path& path) {
    std::fstream file;
    std::stringstream buffer;

    std::cout << "Loading body from: " << path << std::endl;
    // if (!file.is_open())
    //     exitWithError("Could not open file: " + path);
	if (file.is_open()){
		setStatusCode("403");
		std::cout << "hieropenwtf open" << std::endl;
	}
	if (hasAccess(path.c_str(), file)){
		buffer << file.rdbuf();
		_body = buffer.str();
    	file.close();
	}
}

void Response::loadCgi(const Path& path) {
    int         og_StdOut;
    int         pid;
    const char* exc = "/usr/bin/php";
    std::FILE*  temp = std::tmpfile();
    int         fd_temp = fileno(temp);
    std::string flag = "-f";
    char*       argv[] = {const_cast<char *>(exc), const_cast<char *>(flag.c_str()), const_cast<char *>(path.c_str()), nullptr};

    og_StdOut = dup(STDOUT_FILENO);
    pid = fork();
    if (pid == -1)
        exitWithError("Error forking");
    if (pid == 0) {
        std::cout << "Loading CGI from: " << path << std::endl;
        dup2(fd_temp, STDOUT_FILENO);
        execve(exc, argv , nullptr);
    } else {
        waitpid(-1, nullptr, 0);
        dup2(og_StdOut, STDOUT_FILENO);
        lseek(fd_temp, 0, SEEK_SET);
        char buffer[1024];
        long ret = 1;
        while (ret > 0) {
            memset(buffer, 0, 1024);
            ret = read(fd_temp, buffer, 1024);
            _body += buffer;
            std::cout << "Read: " << _body << std::endl;
        }
        setContentLength();
        fclose(temp);
    }
}

void Response::setErrorPage(Path &obj){
	_path = obj;
	//root
	std::string errorpage = "/Users/dyeboa/Documents/Webserv/public/error/" + _statusCode + ".html";
	std::cout << "errorpage = " << errorpage << std::endl;
	std::fstream filestream(errorpage.c_str());
	std::stringstream temp;
  	if (!filestream.is_open() && _statusCode != "404"){
		std::cout << "Error: setErrorPage can't open"  << std::endl;
        _statusCode = "404";
		setErrorPage(_path);
	} else if (!filestream.is_open() && _statusCode == "404"){
		std::cout << "Error: setErrorPage 404 no permission"  << std::endl;
		temp << "<!DOCTYPE html>\n"
				"<html>\n"
				"<div id=main>\n"
				"<div class=fof>\n"
				"<h1>Error 404</h1>\n"
				"</div>\n"
				"</div>\n"
				"</html>\n";
		_body = temp.str();
	}
	else{
		std::cout << "error page to _body"  << std::endl;
		temp << filestream.rdbuf();
		_body = temp.str();
		filestream.close();
	}
}

// std::cout << _responseString << std::endl;
//std::cout << "statuscode: " << _statusCode << "\n" << _contentType << "\r\n" << _contentLength << std::endl;
// std::cout << "content: " << std::string("Content-Length: " + std::to_string(_body.length())) << std::endl;
// std::cout << "content: " << _contentLength << std::endl;

// std::cout << _body << std::endl;
void Response::setResponseString() {
    std::ostringstream ss;
	setContentLength();
	errorCodeMessage();
	setErrorCodeMessage(_statusCode);
	std::cout << "statuscode: " << _statusCode << " statusmessage: " <<  _statusMessage << " content: " << _contentType << " length: " << _contentLength << std::endl;
    ss << _version << " " << _statusCode << " " << _statusMessage << "\r\n" << _contentType << "\r\n" << _contentLength << "\r\n\r\n" << _body;
    _responseString = ss.str();
}

void Response::setErrorCodeMessage(std::string code){
	for (size_t i = 0; i < _errorMessage.size(); i++) {
		if (_errorMessage[i].first == code) {
			_statusMessage = _errorMessage[i].second;
			return ;
		}
	}
	std::cout << "not found statusmessage" << std::endl;
}

//https://developer.mozilla.org/en-US/docs/Web/HTTP/Status
void Response::errorCodeMessage() {
	_errorMessage.push_back(std::make_pair("200", "OK"));
	_errorMessage.push_back(std::make_pair("204", "No Content"));
	_errorMessage.push_back(std::make_pair("307", "Temporary Redirect"));
	_errorMessage.push_back(std::make_pair("400", "Bad Request"));
	_errorMessage.push_back(std::make_pair("401", "Unauthorized"));
	_errorMessage.push_back(std::make_pair("403", "Forbidden"));
	_errorMessage.push_back(std::make_pair("404", "Not Found"));
	_errorMessage.push_back(std::make_pair("405", "Method Not Allowed"));
	_errorMessage.push_back(std::make_pair("408", "Request Timeout"));
	_errorMessage.push_back(std::make_pair("413", "Payload Too Large"));
	_errorMessage.push_back(std::make_pair("414", "URI Too Long"));
	_errorMessage.push_back(std::make_pair("500", "Internal Server Error"));
	_errorMessage.push_back(std::make_pair("503", "Service Unavailable"));
	_errorMessage.push_back(std::make_pair("504", "Gateway Timeout"));
	_errorMessage.push_back(std::make_pair("505", "HTTP Version Not Supported"));
}

void Response::setHeaders(const Path &path) {
    std::string type = MimeTypes::getType(path.c_str());

    this->setStatusCode("200");
    this->setVersion("HTTP/1.1");
    this->setContentType(type);
    this->setContentLength();
}
