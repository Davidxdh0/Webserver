//
// Created by Alle Janse on 6/13/23.
//

#include "Request.h"

Request::Request() {}

Request::Request(const Request &src) {
    *this = src;
}

void Request::parseRequest(std::stringstream &requestRaw) {

    std::string line;


    requestRaw >> _method >> _uri;
    getline(requestRaw, _version, '\r');
    while (line != "\n") {

        getline(requestRaw, line, '\r');
        if (line.empty())
            break;
        std::string::size_type pos = line.find(": ");
        if (pos != std::string::npos) {
            std::string key = line.substr(1, pos);
            std::string value = line.substr(pos + 2);
            _headers[key] = value;
        }
    }
}

Request::~Request() {}