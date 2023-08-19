//
// Created by Alle Janse on 6/13/23.
//

#include "Response.h"
#include "utils.h"
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <cstdio>

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

void Response::loadBody(const Path& path) {
    std::ifstream file(path.c_str());
    std::stringstream buffer;

    std::cout << "Loading body from: " << path << std::endl;
    if (!file.is_open())
        exitWithError("Could not open file: " + path);
    buffer << file.rdbuf();
    _body = buffer.str();
    this->setHeaders("Content-Length: " + std::to_string(_body.length()) + "\r\n");
    file.close();
}

void Response::loadCgi(const Path& path) {
    int         og_StdOut;
    int         pid;
    const char* exc = "/usr/bin/php";
    std::FILE*  temp = std::tmpfile();
    int         fd_temp = fileno(temp);
    char*       argv[] = {const_cast<char *>(exc), const_cast<char *>("-f"), const_cast<char *>(path.c_str()), nullptr};

    og_StdOut = dup(STDOUT_FILENO);
    pid = fork();
    if (pid == -1)
        exitWithError("Error forking");
    if (pid == 0) {
        std::cout << "Loading CGI from: " << path << std::endl;
        dup2(fd_temp, STDOUT_FILENO);
        execve(exc, argv , nullptr);
    } else {
        waitpid(-1, nullptr, 0);
        dup2(og_StdOut, STDOUT_FILENO);
        lseek(fd_temp, 0, SEEK_SET);
        char buffer[1024];
        long ret = 1;
        while (ret > 0) {
            memset(buffer, 0, 1024);
            ret = read(fd_temp, buffer, 1024);
            _body += buffer;
            std::cout << "Read: " << _body << std::endl;
        }
        this->setHeaders("Content-Length: " + std::to_string(_body.length()) + "\r\n");
        fclose(temp);
    }
}

void Response::setResponseString() {
    std::ostringstream ss;

    ss << _version << " " << _statusCode /*<< " " << _statusMessage*/ << "\r\n" << _headers << "\r\n\r\n" << _body;
    _responseString = ss.str();
}