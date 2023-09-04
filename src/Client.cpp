//
// Created by Alle Janse on 6/13/23.
//

#include <iostream>
#include <sstream>
#include <unistd.h>
#include "Client.h"
#include "utils.h"
#include <algorithm>

Client::Client() : _request(), _response(), _socket(), _state(READING) {}

Client::Client(int socket /*int port*/) : _request(), _response(), _socket(socket), _state(READING) /*_port(port)*/ {
}

Client::~Client() {
    close(_socket);
}
std::stringstream&	Client::getRequestRaw(){return _requestRaw;}
const Request&		Client::getRequest()const{return _request;}
const Response&		Client::getResponse()const{return _response;}
const Path&			Client::getPath()const{return _path;}

void Client::handleRequest() {
    this->readRequest();
    if (_state != RESPONDING) {
        return;
    }
	// std::cout << "Requestraw = " <<_requestRaw.str() << std::endl;
    _request.parseRequest(_requestRaw);
    this->configure();
    this->setResponse();
}

int Client::readRequest() {
    char buffer[4096];
    size_t bytes_read;
	size_t content_length = 0;
    size_t t = -1;
	// static int p = 100;

    bytes_read = read(_socket, buffer, sizeof buffer - 1);
    if (bytes_read == t) {
        exitWithError("Error reading from socket");
    }
	else if (bytes_read >= 0)
	{
		buffer[bytes_read] = '\0';
		_requestRaw << buffer;
		std::cout << "1: " <<  buffer << std::endl;
		std::string temp = buffer;
		std::string::size_type pos = temp.find("Content-Length: ");
        if (pos != std::string::npos && content_length >= 0) {
			std::cout << "Readrequest contentlength > 0: " << content_length << std::endl;
            std::string key = temp.substr(1, pos);
			std::stringstream stream(key);
            stream >> content_length;
			std::cout << content_length << std::endl;
		}
		if (content_length > 0){
			std::cout << "Readrequest contentlength > 0: " << content_length << std::endl;
			if (bytes_read <= content_length){
				// _state = RESPONDING;
			}
		}
		else{
			std::cout << "Readrequest normaal" << std::endl;
			if (bytes_read < static_cast<int>(sizeof(buffer)) - 1) {
				_state = RESPONDING;
			}
		}
	}
    return 1;
}

void Client::setResponse() {
    _response.setVersion("HTTP/1.1");
    _response.setStatusCode("200");
    if (_path.getExtension() == "css") {
        _response.setContentType("Content-Type: text/css");
    } else if (_path.getExtension() == "gif") {
        _response.setContentType("Content-Type: image/gif");
    } else {
        _response.setContentType("Content-Type: text/html");
    }
    if (_path.getExtension() == "php") {
        _response.loadCgi(_path);
    } else {
        _response.loadBody(_path);
    }
	if (_request.getUri() == "/upload/upload.php")
		;//_response.upload(_requestRaw);
	if (_request.getMethod() == "DELETE")
		_response.deletePage("/Users/dyeboa/Documents/Webserv/public/AA"); // _path.getFullPath()
	if ( _response.isDirectory(_path.getFullPath())){
		_response.directoryListing(_path.getFullPath());
	}
	if (_response.getStatusCode() != "200"){
		_response.setErrorPage(_path);
	}
    _response.setResponseString();
}

// to check size of response - 
// std::cout << "size = " << _response.getResponseString().size() << std::endl;
// std::cout << "WriteResponseSize = " << _response.getResponseString().size() << std::endl;
void Client::writeResponse() {
	int i = 1;
	if (_response.getResponseString().size() > 64000){
		const char* data = _response.getResponseString().c_str();
		size_t packetsize = 4096;
		size_t dataSent = 0;
		while (dataSent < _response.getResponseString().size()){
			size_t remaining = _response.getResponseString().size() - dataSent;
			size_t currentpacket = std::min(remaining, packetsize);
			i = write(_socket, data + dataSent, currentpacket);
			if (i == -1)
				std::cout << "Write function failed" << std::endl;
			if (i == 0)
				std::cout << "Wrote chunk" << std::endl;
			dataSent += 4096;
		}
	}
	else{
    	i = write(_socket, _response.getResponseString().c_str(), _response.getResponseString().size());
		if (i == -1)
			std::cout << "Write function failed" << std::endl;
		if (i == 0)
			std::cout << "Wrote response" << std::endl;
	}
}

void Client::configure() {
    std::string    root = ROOT;
    std::string    index = INDEX;

    _path = Path(root, _request.getUri());

}