//
// Created by Alle Janse on 6/13/23.
//

#include <iostream>
#include <sstream>
#include <unistd.h>
#include "Client.h"
#include "utils.h"

Client::Client() : _request(), _response() {}

Client::Client(int socket) : _request(), _response(), _socket(socket), _state(READING), _root("/Users/ajanse/Webserv_dev/public") {

}

Client::~Client() {
    close(_socket);
}

void Client::handleRequest() {
    readRequest();
    if (_state != RESPONDING) {
        return;
    }
    _request.parseRequest(_requestRaw);
    this->setResponse();
}

int Client::readRequest() {
    char buffer[24];
    size_t bytes_read;

    bytes_read = read(_socket, buffer, sizeof buffer - 1);
    if (bytes_read == -1) {
        exitWithError("Error reading from socket");
    }
    buffer[bytes_read] = '\0';
    _requestRaw << buffer;
    if (bytes_read < sizeof buffer - 1) {
        _state = RESPONDING;
    }
    return 1;
}

void Client::setResponse() {
    _response.setVersion("HTTP/1.1");
    _response.setStatusCode("200");
    //_response.setStatusMessage("OK");
    _response.setHeaders("Content-Type: text/html");
    _response.loadBody(_root + _request.getUri());
    _response.setResponseString();
}

void Client::writeResponse() {
    write(_socket, _response.getResponseString().c_str(), _response.getResponseString().size());
}