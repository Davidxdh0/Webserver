//
// Created by Alle Janse on 6/13/23.
//

#include "Response.h"
#include "utils.h"
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <cstdio>
#include <dirent.h>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <ctime>

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

void	Response::showDir(){
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir("/Users/dyeboa/Documents/Webserv/public")) == NULL){
		std::cout << "Error: Can't open showDir" << std::endl;
	}
	while ((ent = readdir (dir)) != NULL) {
    	std::cout << "Directory: " << ent->d_name << std::endl;
  	}
  	closedir (dir);
}

/*
Creates an index.html if it can't find an index.html in the desired folder.

todo: 	add checks for autoindex 
		set the correct path.
		Error catching

how to test:
	std::cout << "\n";
	showDir();
	createIndex();
	std::cout << "\n" << std::endl;
*/
void	Response::createIndex(){
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir("/Users/dyeboa/Documents/Webserv/public/error")) == NULL){
		std::cout << "Error: Can't open showDir" << std::endl;
	}
	while ((ent = readdir (dir)) != NULL) {
		if (!strcmp("index.html", ent->d_name)){
    		std::cout << "Directory: index.html found ->" << ent->d_name << std::endl;
			return ;
		}
  	}
	closedir(dir);
	std::ofstream file("/Users/dyeboa/Documents/Webserv/public/error/index1.html");
	file << "<!DOCTYPE html>\n"
			"<html>\n"
			"<head>\n"
			"<title>Index of /</title>\n"
			"<style>\n"
			"table {\n"
			"border-collapse: collapse;\n"
			"width: 100%;\n"
			"}\n"
			"th, td {\n"
			"text-align: left;\n"
			"padding: 8px;\n"
			"}\n"
			"</style>\n"
			"</head>\n"
			"<body>\n"
			"<h1>Index of PATH</h1>\n<table>\n"
			"<tr>\n"
            "<th>Name</th>\n"
            "<th>Date</th>\n"
            "<th>Size</th>\n"
        	"</tr>\n";
	if ((dir = opendir("/Users/dyeboa/Documents/Webserv/public")) == NULL){
		std::cout << "Error: Can't open showDir" << std::endl;
	}
	while ((ent = readdir (dir)) != NULL) {	
		struct stat file_info;
		std::string filepath = std::string("/Users/dyeboa/Documents/Webserv/public") + "/" + ent->d_name;
		std::cout << "Filepath = " << filepath << std::endl;
		file << "<tr>\n";
		if (stat(filepath.c_str(), &file_info) == 0) {
			time_t modified = file_info.st_mtime;
			char time[100];
			std::strftime(time, sizeof(time), "%Y-%m-%d %H:%M:%S", std::localtime(&modified));
			off_t filesize = file_info.st_size;
			file << "<td><a href=" << ent->d_name << ">" << ent->d_name << "</a></td>\n";
			file << "<td>" << time << "</td>\n";
			file << "<td>" << filesize << "</td>\n";
		}
		else{
			file << "<td><a href=" << ent->d_name << ">" << ent->d_name << "</a></td>\n";
			file << "<td>" << "Date not found" << "</td>\n";
			file << "<td>" << "Filesize not found" << "</td>\n";
		}
		file << "</tr>\n";
	}
	closedir(dir);
	file << "</table>\n</body>\n"
			"</html>\n";
	file.close();
}

void	Response::upload(std::stringstream &requestRaw){
	// struct stat file_info;
	std::cout << "\nResponse::upload\n" << std::endl;
	std::cout << "Content of requestRaw:" << std::endl;
	std::cout << requestRaw.str() << std::endl;
	std::string line;
	std::cout << "Content of done:" << std::endl;
    
	// if (stat(file, &file_info) == 0) {
	// 	off_t size = file_info.st_size;
	// 	std::string filepath = std::string("/Users/dyeboa/Documents/Webserv/public/upload/filetoupload") + ".txt";
	// 	std::ofstream uploadfile(filepath);
	// 	size_t packetsize = 4096;
	// 	size_t dataSent = 0;
	// 	while (dataSent < static_cast<size_t>(size)){
	// 		size_t remaining = static_cast<size_t>(size) - dataSent;
	// 		size_t currentpacket = std::min(remaining, packetsize);
	// 		uploadfile <<  file + currentpacket;
	// 		dataSent += 4096;
	// 	}
	// }
	// else {
	// 	std::cout << "Uploading goes wrong" << std::endl;
	// }
}
