//
// Created by Alle Janse on 6/13/23.
//

#include "Request.h"
#include "iostream"
#include <stdlib.h>

Request::Request() : _contentlength(0),_isUpload(0){}

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
//        std::cout << line << std::endl;
        std::string::size_type pos = line.find(": ");
        if (pos != std::string::npos) {
            std::string key = line.substr(1, pos);
            if (key == "Host:") {
                _hostname = line.substr(pos + 2);
            } else if (key == "Content-Type:") {
                _contenttype = line.substr(pos + 2);
				_isUpload = ("multipart/form-data;" == _contenttype.substr(0, 20)) ? 1 : 0;
            } else if (key == "Content-Length:") {
                _contentlength = stol(line.substr(pos + 2));
            }
        }
    }
    requestRaw.get();
    if (_contentlength > 0) {
        char *body = new char[_contentlength + 1];
        requestRaw.read(body, _contentlength);
        body[_contentlength] = '\0';
        _body = body;
        delete[] body;
    }
}

Request::~Request() {}