//
// Created by Alle Janse on 8/22/23.
//

#ifndef WEBSERV_SETTINGS_H
#define WEBSERV_SETTINGS_H

#include <iostream>
#include <map>
#include <string>
#include <vector>

class Settings {

public:
    Settings();
    Settings(const Settings &src);
    ~Settings();

    //Setters
    void setHost(const std::string &host) { _host = host; };
    void setRoot(const std::string &root) { _root = root; };
    void setIndex(const std::string &index) { _index = index; };
    void setAllowMethods(int allow_methods) { _allow_methods = allow_methods; };
    void setAutoindex(std::string  autoindex) { _autoindex = autoindex; };
    void setCgiPath(const std::string &cgi_path) { _cgi_path = cgi_path; };
    void setCgiExtension(const std::string &cgi_extension) { _cgi_extension = cgi_extension; };
    void setUploadPath(const std::string &upload_path) { _upload_path = upload_path; };
    void setClientMaxBodySize(int client_max_body_size) { _client_max_body_size = client_max_body_size; };
    void setErrorPages(const std::vector<std::pair<int, std::string> >  &error_pages) { _error_pages = error_pages; };
	void addErrorPage(const std::pair<int, std::string>& errorPage) { _error_pages.push_back(errorPage); }
	void addLocations(const std::pair<std::string, Settings*> &locations) { _locations.push_back(locations); }
	void setLocations(const std::vector<std::pair<std::string, Settings*> > &locations) { _locations = locations; };
	void setUploadEnable(const int &upload_enable) { _upload_enable = upload_enable; };
	void setReturn(const std::string &reroute) { _return = reroute; };

    //Getters
    const std::string 	&getHost() const { return _host; };
    const std::string 	&getRoot() const { return _root; };
    const std::string 	&getIndex() const { return _index; };
    int 				getAllowMethods() const { return _allow_methods; };
    const std::string	&getAutoindex() const { return _autoindex; };
    const std::string 	&getCgiPath() const { return _cgi_path; };
    const std::string 	&getCgiExtension() const { return _cgi_extension; };
    const std::string 	&getUploadPath() const { return _upload_path; };
    std::vector<std::pair<int, std::string> > &getErrorPages(){ return _error_pages; };
	int					getErrorPageInt() { int p = _error_pages.back().first; return p; };
	const std::string 	getErrorPageString() { std::string p = _error_pages.back().second; return p; };
    std::vector<std::pair<std::string, Settings*> > &getLocations() { return _locations; };
	std::string			getLocationsString() { std::string p = _locations.back().first; _locations.pop_back(); return p; };
	const size_t		&getClientMaxBodySize() const { return _client_max_body_size; };
	const int			&getUploadEnable() const { return _upload_enable; };
	const std::string	&getReturn() const { return _return; };

	// poppers
	
private:

    std::string                     				_host;
    std::string                     				_root;
    std::string                     				_index;
    int                             				_allow_methods;  // 1 = GET, 2 = POST, 4 = DELETE so 7 = GET, POST, DELETE
    std::string                     				_autoindex;
    std::string                     				_cgi_path;
    std::string                     				_cgi_extension;
    std::string                     				_upload_path;
	int												_upload_enable;
	std::string										_return;
    std::vector<std::pair<int, std::string> >  		_error_pages; // 404, 500, 501, 502, 503, 504
    size_t                            				_client_max_body_size;
    std::vector<std::pair<std::string, Settings*> >	_locations;

};
#endif //WEBSERV_SETTINGS_H
