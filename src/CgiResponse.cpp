//
// Created by Alle Janse on 9/13/23.
//
#include "Response.h"
#include <sys/event.h>

void    createPipe(int p[2]) {
    if (pipe(p) < 0)
        exitWithError("Error creating pipe");
}

char**    createEnv(const Path &path, const Request &request) {
    std::string tmp;
    char **env = new char*[12];

    tmp = "REDIRECT_STATUS=200";
    env[0] = new char[tmp.length() + 1];
    strcpy(env[0], tmp.c_str());
    tmp = "SCRIPT_FILENAME=" + path;
    env[1] = new char[tmp.length() + 1];
    strcpy(env[1], tmp.c_str());
    tmp = "CONTENT_LENGTH=" + std::to_string(request.getContentLength());
    env[2] = new char[tmp.length() + 1];
    strcpy(env[2], tmp.c_str());
    tmp = "GATEWAY_INTERFACE=CGI/1.1";
    env[3] = new char[tmp.length() + 1];
    strcpy(env[3], tmp.c_str());
    tmp = "REQUEST_METHOD=" + request.getMethod();
    env[4] = new char[tmp.length() + 1];
    strcpy(env[4], tmp.c_str());
    tmp = "REQUEST_URI=" + request.getUri();
    env[5] = new char[tmp.length() + 1];
    strcpy(env[5], tmp.c_str());
    tmp = "SERVER_PROTOCOL=" + request.getVersion();
    env[6] = new char[tmp.length() + 1];
    strcpy(env[6], tmp.c_str());
    tmp = "SERVER_SOFTWARE=Webserv/1.0";
    env[7] = new char[tmp.length() + 1];
    strcpy(env[7], tmp.c_str());
    tmp = "SERVER_NAME=" + request.getHostname();
    env[8] = new char[tmp.length() + 1];
    strcpy(env[8], tmp.c_str());
    tmp = "SERVER_PORT=8080";
    env[9] = new char[tmp.length() + 1];
    strcpy(env[9], tmp.c_str());
    tmp = "CONTENT_TYPE=" + request.getContentType();
    env[10] = new char[tmp.length() + 1];
    strcpy(env[10], tmp.c_str());
    env[11] = NULL;
    return env;
}

void    Response::parseCgiResponse() {
    std::string::size_type pos = _body.find("\r\n\r\n");
    std::string             cgiHeaders;
    std::string             line;

    if (pos != std::string::npos) {
        cgiHeaders = _body.substr(0, pos + 2);
        _body = _body.substr(pos + 4);
    }
    while (cgiHeaders.find("\r\n") != std::string::npos) {
        pos = cgiHeaders.find("\r\n");
        line = cgiHeaders.substr(0, pos);
        cgiHeaders = cgiHeaders.substr(pos + 2);
        pos = line.find(": ");
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            if (key == "Status") {
                std::string value = line.substr(pos + 2);
                pos = value.find(" ");
                _statusCode = value.substr(0, pos);
                _statusMessage = value.substr(pos + 1);
            } else if (key == "Content-type") {
                this->setContentType(line.substr(pos + 2));
            }
        }
    }
}

//        std::cout << env[i] << std::endl;
void    free_all(char** env) {
    if (!env)
        return;
    for (int i = 0; env[i]; i++) {
        delete env[i];
    }
    delete env;
}

int    Response::loadCgi(const Path &path, const Request &request, const std::string& cgiPath) {
    int        pid;
    int        pipeIn[2];
    int        pipeOut[2];
    char       **env = nullptr;
//    char*       argv[] = {"/usr/bin/php", "-f", const_cast<char *>(path.c_str()), nullptr};


    createPipe(pipeIn);
    createPipe(pipeOut);
    env = createEnv(path, request);
    int i = write(pipeIn[1], request.getBody().c_str(), request.getBody().length());
	if (i == -1)
        exitWithError("write cgi failed");
    close(pipeIn[1]);

    pid = fork();
    if (pid == -1)
        exitWithError("Error forking");
    if (pid == 0) {
        dup2(pipeIn[0], STDIN_FILENO);
        dup2(pipeOut[1], STDOUT_FILENO);
        close(pipeIn[0]);
        close(pipeOut[0]);
        close(pipeOut[1]);
        execve(cgiPath.c_str(), nullptr, env);
    }
    _cgi_pid = pid;
    close(pipeIn[0]);
    close(pipeOut[1]);
    free_all(env);
    return (pipeOut[0]);
}

int    Response::readCgi(int cgi_fd) {
    char buffer[1024];
    long ret = 1;

    memset(buffer, 0, 1024);
    ret = read(cgi_fd, buffer, 1024);
    if (ret == 1024)
        return 1;
    if (ret == -1)
        	exitWithError("readcgi failed");
    if (ret == 0)
        	exitWithError("readcgi failed = 0");
    _body += buffer;
    this->parseCgiResponse();
    return 0;
    }
