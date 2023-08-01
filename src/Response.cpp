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
    std::stringstream buffer;

    std::cout << "Loading body from: " << path << std::endl;
    if (!file.is_open())
        exitWithError("Could not open file: " + path);
    buffer << file.rdbuf();
    _body = buffer.str();
    file.close();
}

void Response::setResponseString() {
    std::ostringstream ss;

    ss << _version << " " << _statusCode /*<< " " << _statusMessage*/ << "\r\n" << _headers << "\r\n\r\n" << _body;
    _responseString = ss.str();
}