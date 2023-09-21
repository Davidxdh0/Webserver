//
// Created by Alle Janse on 6/13/23.
//

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include "Client.h"
#include "utils.h"
#include <algorithm>

Client::Client() : _socket(), _state(READING), _vhosts() {}

Client::Client(int socket, Settings* vhosts) : _socket(socket), _state(READING), _vhosts(vhosts) {
}

Client::~Client() {
    close(_socket);
}

std::stringstream&	Client::getRequestRaw(){return _requestRaw;}
const Request&		Client::getRequest()const{return _request;}
const Response&		Client::getResponse()const{return _response;}
const Path&			Client::getPath()const{return _path;}

void Client::handleRequest(long data) {
    this->readRequest(data);
    if (_state != RESPONDING)
        return;
    _request.parseRequest(_requestRaw);
    this->configure();
    this->redirect();
    this->setResponse();
}

int Client::readChunked(std::string bufferstring, int chunkedrequest){
    std::string::size_type head = bufferstring.find("\r\n\r\n");
    std::string::size_type headchunked = bufferstring.find("Transfer-Encoding: chunked");
    if (headchunked < head) {
        chunkedrequest = 1;
        _chunked = "on";
    }
    else
        chunkedrequest = 2;
    if (bufferstring.find("\r\n0\r\n\r\n") != std::string::npos)
        chunkedrequest = 2;
    return chunkedrequest;
}

int Client::readRequest(long data) {
    char buffer[1024];
    size_t bytes_read;
    size_t t = -1;
    std::string bufferstring;
    static int chunkedrequest = 0;

    bytes_read = read(_socket, buffer, sizeof buffer - 1);
    if (bytes_read == t)
        exitWithError("Error reading from socket");
	else {
        buffer[bytes_read] = '\0';

        bufferstring = buffer;
        chunkedrequest = readChunked(bufferstring, chunkedrequest);
        if ((bytes_read < sizeof buffer - 1 || bytes_read == static_cast<size_t>(data)) && chunkedrequest != 1)
            _state = RESPONDING;
        _requestRaw.write(buffer, bytes_read);
    }
    return 1;
}
// std::cout << "body" << _response.getBody().size() << std::endl;
// std::cout << "code: " << _response.getStatusCode()  << std::endl;
// std::cou << " autoindex: " << _settings.getAutoindex() << std::endl;
void Client::setResponse() {

    this->checkMethod();
    if (_path.isDirectory()) {
        this->index();
    }
    if (!_path.getExtension().empty()&& _path.getExtension() == _settings.getCgiExtension()) {
        _response.loadCgi(_path, _request, _settings.getCgiPath());
    } else {
        _response.loadBody(_path);
    }
    if (_request.getisUpload())
	 	_response.uploadFile(_requestRaw, _vhosts, _request.getContentType());
    if (_request.getMethod() == "DELETE")
        _response.deletePage(_path.getFullPath(), &_settings);
    if (_response.getStatusCode() != "200") {
        _response.errorCodeMessage();
        _response.setErrorCodeMessage(_response.getStatusCode());
        _response.setErrorPage(_settings.getRoot(), getErrorPath());
    }
    _response.setHeaders(_path);
    _response.setResponseString();
}

void Client::writeResponse() {
	int i = 1;
    size_t packetsize = 10000;
    static size_t dataSent = 0;
	if (_response.getResponseString().size() - dataSent > packetsize){
		const char* data = _response.getResponseString().c_str();
        size_t remaining = _response.getResponseString().size() - dataSent;
        size_t currentpacket = std::min(remaining, packetsize);
        i = write(_socket, data + dataSent, currentpacket);
        dataSent += i;
	}
	else{
    	i = write(_socket, _response.getResponseString().c_str(), _response.getResponseString().size());
	}
    if (i == -1)
        exitWithError("writing Response failed");
    if (i == 0)
        exitWithError("writing Response failed = 0");
}

void Client::configure() {

    Settings ret;
    Path uri(_request.getUri());
    std::string host    = _request.getHostname();
    std::string port    = host.substr(host.find(":"));
//    std::cout << _vhosts->getHost() + port << " Host:" << host << std::endl;
    std::cout << "Host: " << host << std::endl;
//    setLocal(port);
    ret = _vhosts->getRightSettings(uri);
    _settings = ret;
    _path = _settings.getRoot() + _request.getUri();
}

//std::cout << "redir      path: " << _path << std::endl;
//std::cout << "redir      path: " << _path.getFullPath() << std::endl;
//std::cout << "new redire path: " << _settings.getRoot() + _settings.getAlias() + "/" + _path.getFilename() << std::endl;
void Client::redirect(){
    if (_settings.getAlias() == "")
        return ;
    _path.setFullPath(_settings.getRoot() + _settings.getAlias() + "/" + _path.getFilename());
}

void Client::checkMethod() {
    std::string method = _request.getMethod();
    int allowed = _settings.getAllowMethods();
    int method_i = 0;

    if (method == "GET") {
        method_i = 1;
    } else if (method == "POST") {
        method_i = 2;
    } else if (method == "DELETE") {
        method_i = 4;
    }
    if ((allowed & method_i) == 0)
        _response.setStatusCode("405");
}

void Client::index() {

    std::string index_path;
    if (_settings.getIndex() != "")
        index_path = _path.getFullPath() + _settings.getIndex();
    if (access(index_path.c_str(), F_OK) == -1) {
        if (_settings.getAutoindex() == "on") {
            _response.directoryListing(_path.getFullPath(), _settings.getIndex());
        } else {
            _response.setStatusCode("403");
        }
    } else {
        Path indexHtml(index_path);
        _path = index_path;

    }
}

void Client::setLocal(std::string &port) {

    if (_request.getHostname() == "localhost" + port && _vhosts->getHost() == "127.0.0.1")
        _vhosts->setHost("localhost");
    if (_request.getHostname() == "127.0.0.1" + port && _vhosts->getHost() == "localhost")
        _vhosts->setHost("127.0.0.1");
}

//std::cout << "code: " << _response.getStatusCode() << std::endl;
std::string Client::getErrorPath() {
    int code = std::stoi(_response.getStatusCode());
    for (size_t i = 0; i < _settings.getErrorPages().size(); i++){
        if (code == _settings.getErrorPages()[i].first)
            return _settings.getErrorPages()[i].second;
    }
    return "Not found in ErrorPages -> check config";
}