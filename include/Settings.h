//
// Created by Alle Janse on 8/22/23.
//

#ifndef WEBSERV_SETTINGS_H
#define WEBSERV_SETTINGS_H

#include <iostream>
#include <map>

class Settings {

public:
    Settings();
    Settings(const Settings &src);
    ~Settings();

private:

    std::string                     _host;
    std::string                     _root;
    std::string                     _index;
    int                             _allow_methods;  // 1 = GET, 2 = POST, 4 = DELETE so 7 = GET, POST, DELETE
    bool                            _autoindex;
    std::string                     _cgi_path;
    std::string                     _cgi_extension;
    std::string                     _upload_path;
    std::map<std::string, Settings> _locations;

};
#endif //WEBSERV_SETTINGS_H
