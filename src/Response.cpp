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

Response::Response() : _statusCode("200"), _responseString("") {}

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
	if (file.is_open()){
		setStatusCode("403");
		return ;
	}
	if (hasAccess(path.c_str(), file) && path.getFullPath()[path.getFullPath().length() - 1] != '/'){
		buffer << file.rdbuf();
		_body = buffer.str();
    	file.close();
	}
}

//	std::cout << "errorpage = " << errorpage << std::endl;
//	std::cout << "Error: setErrorPage can't open"  << std::endl;
//	std::cout << "error page to _body"  << std::endl;
void Response::setErrorPage(std::string root, std::string errorpage){
    std::string path = root + "/" + errorpage;
	std::fstream filestream(path.c_str());
	std::stringstream temp;
  	if (!filestream.is_open()){
		temp << "<!DOCTYPE html>\n"
				"<html>\n"
				"<div id=main>\n"
				"<div class=fof>\n"
				"<h1>Error " + getStatusCode() + " </h1>\n"
				"</div>\n"
				"</div>\n"
				"</html>\n";
		_body = temp.str();
	}
	else{
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
	
	// std::cout << "statuscode: " << _statusCode << " headers: " << _headers  << " statusmessage: " <<  _statusMessage << " content: " << _contentType << " length: " << _contentLength << std::endl;
    ss << _version << " " << _statusCode << " "  << _statusMessage << "\r\n" << _contentType << "\r\n"  << _headers << _contentLength << "\r\n\r\n" << _body;
    _responseString = ss.str();
}

// std::cout << "found statusmessage: " << _errorMessage[i].second << std::endl;
void Response::setErrorCodeMessage(std::string code){
	for (size_t i = 0; i < _errorMessage.size(); i++) {
		if (_errorMessage[i].first == code) {
			_statusMessage = _errorMessage[i].second;
			return ;
		}
	}
	std::cout << "ErrorMessage not in errorCodeMessage()" << std::endl;
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

    this->setVersion("HTTP/1.1");
    if (path.getExtension() != "php")
        this->setContentType(type);
    this->setContentLength();
}
