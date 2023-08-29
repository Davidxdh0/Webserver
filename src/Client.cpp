//
// Created by Alle Janse on 6/13/23.
//

#include <iostream>
#include <sstream>
#include <unistd.h>
#include "Client.h"
#include "utils.h"

Client::Client() : _request(), _response(), _socket(), _state(READING) {}

Client::Client(int socket) : _request(), _response(), _socket(socket), _state(READING) {}

Client::~Client() {
    close(_socket);
}

void	Client::handleRequest() {
    readRequest();
    if (_state != RESPONDING) {
        return;
    }
    _request.parseRequest(_requestRaw);
    _path = Path("/Users/dyeboa/Documents/Webserv/public", _request.getUri());
    this->setResponse();
}

int Client::readRequest() {
    char buffer[24];
    int bytes_read;

    bytes_read = read(_socket, buffer, sizeof buffer - 1);
    if (bytes_read == -1) {
        exitWithError("Error reading from socket");
    }
    buffer[bytes_read] = '\0';
    _requestRaw << buffer;
    if (bytes_read < static_cast<int>(sizeof(buffer)) - 1) {
        _state = RESPONDING;
    }
    return 1;
}

void Client::setResponse() {
    _response.setVersion("HTTP/1.1");
    _response.setStatusCode("200");
    if (_path.getExtension() == "css") {
        _response.setHeaders("Content-Type: text/css");
    } else if (_path.getExtension() == "gif") {
        _response.setHeaders("Content-Type: image/gif");
    } else {
        _response.setHeaders("Content-Type: text/html");
    }
    if (_path.getExtension() == "php") {
        _response.loadCgi(_path);
    } else {
        _response.loadBody(_path);
    }
	if (_request.getUri() == "/upload/upload.php")
		_response.upload(_requestRaw);
	if (_request.getUri() == "/delete_file?filename=index.html")
		_response.deletePage();
    _response.setResponseString();
}

// to check size of response - 
// std::cout << "size = " << _response.getResponseString().size() << std::endl;
void Client::writeResponse() {
	// std::cout << "WriteResponseSize = " << _response.getResponseString().size() << std::endl;
	if (_response.getResponseString().size() > 64000){
		const char* data = _response.getResponseString().c_str();
		size_t packetsize = 4096;
		size_t dataSent = 0;
		while (dataSent < _response.getResponseString().size()){
			size_t remaining = _response.getResponseString().size() - dataSent;
			size_t currentpacket = std::min(remaining, packetsize);
			write(_socket, data + dataSent, currentpacket);
			//check kque
			dataSent += 4096;
		}
	}
	else
    	write(_socket, _response.getResponseString().c_str(), _response.getResponseString().size());
}
