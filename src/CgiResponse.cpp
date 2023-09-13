//
// Created by Alle Janse on 9/13/23.
//
#include "Response.h"

void    createPipe(int p[2]) {
    if (pipe(p) < 0)
        exitWithError("Error creating pipe");
}

void    createEnv(char** env, const Path &path, const Request &request) {
    std::string tmp;
    env = (char**)malloc(sizeof(char*) * 12);

    tmp = "REDIRECT_STATUS=200";
    env[0] = strdup(tmp.c_str());
    tmp = "SCRIPT_FILENAME=/usr/bin/php";
    env[1] = strdup(tmp.c_str());
    tmp = "Content-Length=" + std::to_string(request.getContentLength());
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
    tmp = "Content-Type=" + request.getContentType();
    env[10] = strdup(tmp.c_str());
    env[11] = NULL;
}

void    free_all(char** env) {
    if (!env)
        return;
    for (int i = 0; env[i]; i++)
        free(env[i]);
    free(env);
}

void    Response::loadCgi(const Path &path, const Request &request) {
    int        pid;
    int        pipeIn[2];
    int        pipeOut[2];
    char       **env = NULL;
    char*       argv[] = {"/usr/bin/php", "-f", const_cast<char *>(path.c_str()), nullptr};


    createPipe(pipeIn);
    createPipe(pipeOut);
    createEnv(env, path, request);
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
        execve("/usr/bin/php", nullptr, env);
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