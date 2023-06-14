//
// Created by Alle Janse on 6/13/23.
//

#include "Response.h"
#include "utils.h"
#include <fstream>
#include <sstream>

Response::Response() {}

Response::~Response() {}

void Response::setVersion(const std::string &version) {
    _version = version;
}

void Response::setStatusCode(const std::string &statusCode) {
    _statusCode = statusCode;
}

void Response::setStatusMessage(const std::string &statusMessage) {
    _statusMessage = statusMessage;
}

void Response::setHeaders(const std::string &headers) {
    _headers = headers;
}

void Response::loadBody(const std::string &path) {
    std::ifstream file(path.c_str());

    if (!file.is_open())
        exitWithError("Could not open file: " + path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    _body = buffer.str();
    file.close();
}

std::string Response::getResponse() {
    std::ostringstream ss;

    ss << _version << " " << _statusCode << " " << _statusMessage << "\r\n" << _headers << "\r\n\r\n" << _body;
    return ss.str();
}