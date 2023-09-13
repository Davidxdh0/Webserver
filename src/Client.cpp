//
// Created by Alle Janse on 6/13/23.
//

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include "Client.h"
#include "utils.h"
#include <algorithm>

Client::Client() : _socket(), _state(READING), _vhosts() {}

Client::Client(int socket, Settings* vhosts) : _socket(socket), _state(READING), _vhosts(vhosts) {
}

Client::~Client() {
    close(_socket);
}

std::stringstream&	Client::getRequestRaw(){return _requestRaw;}
const Request&		Client::getRequest()const{return _request;}
const Response&		Client::getResponse()const{return _response;}
const Path&			Client::getPath()const{return _path;}

void Client::handleRequest(long data) {
    this->readRequest(data);
    if (_state != RESPONDING)
        return;
    _request.parseRequest(_requestRaw);
    this->configure();
    this->setResponse();
}

int Client::readRequest(long data) {
    char buffer[1024];
    size_t bytes_read;
    size_t t = -1;
	std::string bufferstring;
//	static size_t content_length = 0;
	// static int chunkedrequest = 0;

    bytes_read = read(_socket, buffer, sizeof buffer - 1);
    if (bytes_read == t)
        exitWithError("Error reading from socket");
	else {
        buffer[bytes_read] = '\0';
		// bufferstring = buffer;
		// if (chunkedrequest == -1){
		// 	if (bufferstring.find("\r\n\r\n") != std::string::npos)
		// 		chunkedrequest = 1;
		// }
		// if (chunkedrequest == 0){
		// 	if (bufferstring.find("Transfer-Encoding: chunked") != std::string::npos)
		// 		chunkedrequest = 1;
		// }
		// if (chunkedrequest == 1){
		// 	std::string body;
		// 	size_t bodychunked = bufferstring.find("\r\n\r\n");
		// 	if (bodychunked != std::string::npos)
		// 		std::string body = bufferstring.substr(bodychunked + 4);
		// 	else
		// 		body = bufferstring;
		// 	std::istringstream iss(body);
		// 	// std::cout << iss.str() << std::endl;
		// 	std::string chunksizestr;
		// 	std::string chunkstring;
		// 	int chunksize = -1;
		// 	while(true){
		// 		if (!std::getline(iss, chunksizestr))
		// 			break;//exitWithError("Error istringstream getline");
		// 		std::cout << chunksizestr << std::endl;
		// 		chunksize = std::stoi(chunksizestr, nullptr, 16);
		// 		//  std::cout << "Chunksize: " <<chunksize << std::endl;
		// 		if (chunksize == 0)
		// 			chunkedrequest = 0;
		// 		chunkstring.resize(chunksize);
		// 		if (!iss.read(&chunkstring[0], chunksize))
		// 			exitWithError("Error reading istringstream");
		// 		iss.ignore(2);
		// 	}
		// 	if (chunksize == 0)
		// 		chunkstring += "\r\nEOF";
		// 	// std::cout << "Chunk Size: " << chunksize << std::endl;
		// 	// std::cout << "Chunk Data: " << chunkstring << std::endl;
		// }
        if (bytes_read < sizeof buffer - 1 || bytes_read == static_cast<size_t>(data))
            _state = RESPONDING;
        _requestRaw.write(buffer, bytes_read);
		// std::cout << _requestRaw.str() << std::endl;
        // _requestRaw << buffer;
    }
    return 1;
}

void Client::setResponse() {
    this->checkMethod();
    if (_path.isDirectory()) {
        this->index();
    }
    if (_path.getExtension() == "php") {
        _response.loadCgi(_path);
    } else {
        _response.loadBody(_path);
    }
	// if (_request.getisUpload())
	// 	_response.uploadFile(_requestRaw, _vhosts);

    _response.setHeaders(_path);
    _response.setResponseString();
}

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

    Settings ret;
    std::string host = _request.getHostname();
    Path uri(_request.getUri());

    int i = 0;
    while(!_vhosts[i].getHost().empty()) {
        if (_vhosts[i].getHost() == host) {
            ret = _vhosts[i].getRightSettings(uri);
            break;
        }
        i++;
    }
    _settings = ret;
    _path = _settings.getRoot() + _request.getUri();
}

void Client::checkMethod() {
    std::string method = _request.getMethod();
    int allowed = _settings.getAllowMethods();
    int method_i = 0;

    if (method == "GET") {
        method_i = 1;
    } else if (method == "POST") {
        method_i = 2;
    } else if (method == "DELETE") {
        method_i = 4;
    }
    if ((allowed & method_i) == 0)
        _response.setStatusCode("405");
}

void Client::index() {
    std::string index_path = _path.getFullPath() + _settings.getIndex();

    if (access(index_path.c_str(), F_OK) == -1) {
        if (_settings.getAutoindex() == "on") {
            _response.directoryListing(_path.getFullPath());
        } else {
            _response.setStatusCode("403");
        }
    } else {
        _path = index_path;
    }
}

