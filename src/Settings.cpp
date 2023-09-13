//
// Created by Alle Janse on 8/29/23.
//

#include "Settings.h"

Settings::Settings() : _allow_methods(0), _autoindex(), _client_max_body_size(1000000)
{
}

Settings::Settings(const Settings &src) : _allow_methods(), _autoindex(), _client_max_body_size()
{
    *this = src;
}

Settings::~Settings()
{
}

Settings Settings::getRightSettings(Path &uri) {
    Settings ret(*this);
    if (!_locations.empty()){
        std::vector<std::pair<std::string, Settings* > >::iterator it = _locations.begin();

        while (it != _locations.end()) {
            if (uri.getFullPath().find(it->first) != std::string::npos) {
                ret = *it->second;
                break;
            }
            it++;
        }
    }
    return ret;
}


