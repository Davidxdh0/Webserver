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
    std::string     getHostname() { return _hostname; };
	const bool		&getisUpload() const { return _isUpload;};
    const std::string &getContentType() const { return _contenttype;}
    std::string     getBody() const { return _body; };
    long            getContentLength() const { return _contentlength; };
private:

//    std::map<std::string, std::string>  _headers;
    std::string                         _method;
    std::string                         _uri;
    std::string                         _version;
    std::string                         _hostname;
    std::string                         _contenttype;
    std::string                         _body;
	bool								_isUpload;
    long                                _contentlength;

};


#endif //WEBSERV_REQUEST_H
