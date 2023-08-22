//
// Created by Alle Janse on 6/13/23.
//

#ifndef WEBSERV_REQUEST_H
#define WEBSERV_REQUEST_H

#include <sstream>
#include <map>

class Request {

public:
    Request();
    Request(const Request &src);
    ~Request();

    void            parseRequest(std::stringstream &requestRaw);

    std::string     getMethod() { return _method; };
    std::string     getUri() { return _uri; };
    std::string     getVersion() { return _version; };

private:

//    std::map<std::string, std::string>  _headers;
    std::string                         _method;
    std::string                         _uri;
    std::string                         _version;
    std::string                         _hostname;

};


#endif //WEBSERV_REQUEST_H
