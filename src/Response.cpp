//
// Created by Alle Janse on 6/13/23.
//

#include "Response.h"
#include "Client.h" 
#include "utils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <cstdio>
#include <dirent.h>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <ctime>
#include <vector>

Response::Response() {}

Response::~Response() {}

void	Response::setVersion(const std::string &version) {_version = version;}
void	Response::setStatusCode(const std::string &statusCode) {_statusCode = statusCode;}
void	Response::setStatusMessage(const std::string &statusMessage) {_statusMessage = statusMessage;}
void	Response::setHeaders(const std::string &headers) {_headers = headers;}

std::string	Response::getStatusCode(void) {return _statusCode;}
std::string	Response::getStatusMessage(void) {return _statusMessage;}

void Response::loadBody(const Path& path) {
    std::ifstream file(path.c_str());
    std::stringstream buffer;

    std::cout << "Loading body from: " << path << std::endl;
    if (!file.is_open())
        exitWithError("Could not open file: " + path);
    buffer << file.rdbuf();
    _body = buffer.str();
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

void Response::setErrorPage(Path &obj){
	_path = obj;
	//root
	std::string errorpage = "/Users/dyeboa/Documents/Webserv/public/error/" + _statusCode + ".html";
	std::cout << "errorpage = " << errorpage << std::endl;
	std::ifstream filestream(errorpage.c_str());
	std::stringstream temp;
  	if (!filestream.is_open() && _statusCode != "404"){
		std::cout << "Error: setErrorPage can't open"  << std::endl;
        _statusCode = "404";
		setErrorPage(_path);
	} else if (!filestream.is_open() && _statusCode == "404"){
		std::cout << "Error: setErrorPage 404 no permission"  << std::endl;
		temp << "<!DOCTYPE html>\n"
				"<html>\n"
				"<div id=main>\n"
				"<div class=fof>\n"
				"<h1>Error 404</h1>\n"
				"</div>\n"
				"</div>\n"
				"</html>\n";
		_body = temp.str();
	}
	else{
		std::cout << "error page to _body"  << std::endl;
		temp << filestream.rdbuf();
		_body = temp.str();
	}
}

void Response::setResponseString() {
    std::ostringstream ss;
	// std::cout << "Body: " << _body << std::endl;
	// std::cout << "statuscode: " << _statusCode << "\nHeaders: " <<_headers << std::endl;
	this->setHeaders("Content-Length: " + std::to_string(_body.length() + 1) + "\r\n");
    ss << _version << " " << _statusCode << " " << "OK" << "\r\n" << "Content-Type: text/html\r\n" <<_headers << "\r\n\r\n" << _body;
    _responseString = ss.str();
	// std::cout << _responseString << std::endl;
}

// void Response::errorCode() {
// 	std::vector<std::pair<int, std::string> >	error;
// 	error.push_back(std::make_pair(204, "204"));
// 	error.push_back(std::make_pair(307, "307"));
// 	error.push_back(std::make_pair(400, "400"));
// 	error.push_back(std::make_pair(401, "401"));
// 	error.push_back(std::make_pair(403, "403"));
// 	error.push_back(std::make_pair(404, "404"));
// 	error.push_back(std::make_pair(405, "405"));
// 	error.push_back(std::make_pair(408, "408"));
// 	error.push_back(std::make_pair(413, "413"));
// 	error.push_back(std::make_pair(414, "414"));
// 	error.push_back(std::make_pair(500, "500"));
// 	error.push_back(std::make_pair(503, "503"));
// 	error.push_back(std::make_pair(504, "504"));
// 	error.push_back(std::make_pair(505, "505"));
// }