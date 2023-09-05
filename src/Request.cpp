//
// Created by Alle Janse on 6/13/23.
//

#include "Request.h"
#include "iostream"

Request::Request() {}

Request::Request(const Request &src) {
    *this = src;
}

void Request::parseRequest(std::stringstream &requestRaw, std::string& contenttype) {
    std::string line;

    requestRaw >> _method >> _uri >> _version;
    getline(requestRaw, line, '\r');
    while (line != "\n") {
        getline(requestRaw, line, '\r');
        if (line.empty())
            break;
		// std::cout << line << std::endl;
        std::string::size_type pos = line.find(": ");
        if (pos != std::string::npos) {
            std::string key = line.substr(1, pos);
            if (key == "Host:")
            {
                _hostname = line.substr(pos + 2);
            }
			else if (key == "Content-Type:"){
				contenttype = line.substr(pos + 2);
				// std::cout << contenttype << std::endl;
			}
        }
    }
	requestRaw.seekg(0);
	std::cout << "contenttype == " << contenttype << std::endl;
    std::cout << "Method: " << _method << std::endl;
    std::cout << "Uri: " << _uri << std::endl;
    std::cout << "Version: " << _version << std::endl;
    std::cout << "Hostname: " << _hostname << std::endl;
	if (_hostname == "ssds")
		contenttype = "lol";
}

Request::~Request() {}