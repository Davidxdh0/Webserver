//
// Created by Alle Janse on 9/13/23.
//
#include "Response.h"

void    createPipe(int p[2]) {
    if (pipe(p) < 0)
        exitWithError("Error creating pipe");
}

char**    createEnv(const Path &path, const Request &request) {
    std::string tmp;
    char **env = (char**)malloc(sizeof(char*) * 12);

    tmp = "REDIRECT_STATUS=200";
    env[0] = strdup(tmp.c_str());
    tmp = "SCRIPT_FILENAME=" + path;
    env[1] = strdup(tmp.c_str());
    tmp = "CONTENT_LENGTH=" + std::to_string(request.getContentLength());
    env[2] = strdup(tmp.c_str());
    tmp = "GATEWAY_INTERFACE=CGI/1.1";
    env[3] = strdup(tmp.c_str());
    tmp = "REQUEST_METHOD=" + request.getMethod();
    env[4] = strdup(tmp.c_str());
    tmp = "REQUEST_URI=" + request.getUri();
    env[5] = strdup(tmp.c_str());
    tmp = "SERVER_PROTOCOL=" + request.getVersion();
    env[6] = strdup(tmp.c_str());
    tmp = "SERVER_SOFTWARE=Webserv/1.0";
    env[7] = strdup(tmp.c_str());
    tmp = "SERVER_NAME=" + request.getHostname();
    env[8] = strdup(tmp.c_str());
    tmp = "SERVER_PORT=8080";
    env[9] = strdup(tmp.c_str());
    tmp = "CONTENT_TYPE=" + request.getContentType();
    env[10] = strdup(tmp.c_str());
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
void    free_all(char** env) {
    if (!env)
        return;
    for (int i = 0; env[i]; i++) {
        std::cout << env[i] << std::endl;
        free(env[i]);
    }
    free(env);
}

void    Response::loadCgi(const Path &path, const Request &request, const std::string& cgiPath) {
    int        pid;
    int        pipeIn[2];
    int        pipeOut[2];
    char       **env = nullptr;
//    char*       argv[] = {"/usr/bin/php", "-f", const_cast<char *>(path.c_str()), nullptr};


    createPipe(pipeIn);
    createPipe(pipeOut);
    env = createEnv(path, request);
    write(pipeIn[1], request.getBody().c_str(), request.getBody().length());
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
    } else {
        waitpid(-1, nullptr, 0);
        close(pipeIn[0]);
        close(pipeOut[1]);
        char buffer[1024];
        long ret = 1;
        while (ret > 0) {
            memset(buffer, 0, 1024);
            ret = read(pipeOut[0], buffer, 1024);
            _body += buffer;
        }
        parseCgiResponse();
        free_all(env);
    }
}


//void Response::loadCgi(const Path& path, const Request& request) {
//    int         og_StdOut;
//    int         pid;
//    const char* exc = "/usr/bin/php";
//    std::FILE*  temp = std::tmpfile();
//    int         fd_temp = fileno(temp);
//    std::string flag = "-f";
//    char*       argv[] = {const_cast<char *>(exc), const_cast<char *>(flag.c_str()), const_cast<char *>(path.c_str()), nullptr};
//
//    og_StdOut = dup(STDOUT_FILENO);
//    pid = fork();
//    if (pid == -1)
//        exitWithError("Error forking");
//    if (pid == 0) {
//        std::cout << "Loading CGI from: " << path << std::endl;
//        write(STDIN_FILENO, request.getBody().c_str(), request.getBody().length());
//        dup2(fd_temp, STDOUT_FILENO);
//        execve(exc, argv , nullptr);
//    } else {
//        waitpid(-1, nullptr, 0);
//        dup2(og_StdOut, STDOUT_FILENO);
//        lseek(fd_temp, 0, SEEK_SET);
//        char buffer[1024];
//        long ret = 1;
//        while (ret > 0) {
//            memset(buffer, 0, 1024);
//            ret = read(fd_temp, buffer, 1024);
//            _body += buffer;
//            std::cout << "Read: " << _body << std::endl;
//        }
//        setContentLength();
//        fclose(temp);
//    }
//}