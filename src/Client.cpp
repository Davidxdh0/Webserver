//
// Created by Alle Janse on 6/13/23.
//

#include <iostream>
#include <sstream>
#include <unistd.h>
#include "Client.h"
#include "utils.h"

Client::Client() : _request(), _response() {}

Client::Client(int socket) : _request(), _response(), _socket(socket) {

}

Client::~Client() {
    close(_socket);
}

void Client::handleRequest() {
    std::stringstream ss;

    readRequest(ss);
    std::cout << ss.str() << std::endl;
}

int Client::readRequest(std::stringstream &ss) {
    char buffer[24];
    int bytes_read;

    bytes_read = read(_socket, buffer, sizeof buffer - 1);
    if (bytes_read == -1) {
        exitWithError("Error reading from socket");
    }
    buffer[bytes_read] = '\0';
    ss << buffer;
    if (bytes_read == sizeof buffer - 1) {
        readRequest(ss);
    }
    return 1;
}

void Client::sendResponse() {
    std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your ServerBlock :) </p></body></html>";
    std::ostringstream ss;
    ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
       << htmlFile;

    write(_socket, ss.str().c_str(), ss.str().size());
}