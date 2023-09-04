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
        std::string::size_type pos = line.find(": ");
        if (pos != std::string::npos) {
            std::string key = line.substr(1, pos);
            if (key == "Host:")
            {
                _hostname = line.substr(pos + 2);
            }
//            std::string value = line.substr(pos + 2);
//           _headers[key] = value;
        }
    }
    std::cout << "Method: " << _method << std::endl;
    std::cout << "Uri: " << _uri << std::endl;
    std::cout << "Version: " << _version << std::endl;
    std::cout << "Hostname: " << _hostname << std::endl;
}

Request::~Request() {}