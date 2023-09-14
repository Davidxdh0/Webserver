//
// Created by Alle Janse on 8/29/23.
//

#include "Settings.h"

Settings::Settings() : _allow_methods(-1), _autoindex(), _upload_enable(-1), _client_max_body_size(1)
{
}

Settings::Settings(const Settings &src) : _allow_methods(), _autoindex(), _client_max_body_size()
{
    *this = src;
}

Settings::~Settings()
{
}
int	Settings::countSlash(std::string location){
	int count = 0;
	for(size_t i = 0; i < location.length(); i++){
		if (location[i] == '/'){
			while (location[i] == '/')
				i++;
			count++;
		}
	}
	return count;
}

Settings Settings::getRightSettings(Path &uri) {
	// int urislash = countSlash(uri.getFullPath());
	// int locationslash = 0;
    Settings ret(*this);
    std::cout << "getRightSettings:" << uri.getFullPath() << std::endl;
    std::cout << "getRightSettings:" << uri.getExtension() << std::endl;
    std::cout << "getRightSettings:" << uri.getFilename() << std::endl;
//    std::cout << "getRightSettings:" << std::endl;
    if (!_locations.empty() != 0){
        std::vector<std::pair<std::string, Settings* > >::iterator it = _locations.begin();
        while (it != _locations.end()) {
            std::cout << "Location: " << getRoot() + uri.getFullPath() << " : " << it->first << std::endl;
            if (uri.getFullPath().find(it->first) != std::string::npos) {
				std::cout << "Found settings" << std::endl;
                ret = *it->second;
                return ret;
                break;
            }
            it++;
        }
    }
    exit(1);
    return ret;
}


