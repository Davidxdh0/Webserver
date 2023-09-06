//
// Created by Alle Janse on 6/13/23.
//

#include "Request.h"
#include "iostream"

Request::Request() {}

Request::Request(const Request &src) {
    *this = src;
}

void Request::parseRequest(std::stringstream &requestRaw) {
    std::string line;

    requestRaw >> _method >> _uri >> _version;
    getline(requestRaw, line, '\r');
    while (line != "\n") {
        getline(requestRaw, line, '\r');
        if (line.empty())
            break;
		std::cout << line << std::endl;
        std::string::size_type pos = line.find(": ");
        if (pos != std::string::npos) {
            std::string key = line.substr(1, pos);
            if (key == "Host:") {
                _hostname = line.substr(pos + 2);
            } else if (key == "Content-Type:") {
                _contenttype = line.substr(pos + 2);
            }
        }
    }
}

Request::~Request() {}