//
// Created by Alle Janse on 6/13/23.
//

#ifndef WEBSERV_RESPONSE_H
#define WEBSERV_RESPONSE_H


#include <string>

class Response {

public:
    Response();
    ~Response();

    void            setVersion(const std::string &version);
    void            setStatusCode(const std::string &statusCode);
    void            setStatusMessage(const std::string &statusMessage);
    void            setHeaders(const std::string &headers);
    void            loadBody(const std::string &path);
    std::string     getResponse();

private:

    std::string     _version;
    std::string     _statusCode;
    std::string     _statusMessage;
    std::string     _headers;
    std::string     _body;
};


#endif //WEBSERV_RESPONSE_H
