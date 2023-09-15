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

void    Settings::removeDir(std::string &search){
    int slashCount = countSlash(search);
    if (slashCount > 1)
        search = search.substr(0, search.find_last_of("/"));
    else if (search.length() > 1)
        search = search.substr(0, search.find_last_of("/") + 1);
    else
        search = "";
}

Settings Settings::getRightSettings(Path &uri) {
    std::vector<std::pair<std::string, Settings* > >::iterator it;
    std::string search;
    std::string fullpath;
    Settings ret(*this);

    if (uri.getFullPath()[uri.getFilename().length()] == '/')
        search = uri.getFullPath().substr(0, uri.getFullPath().length() - 1);
    else
        search = uri.getFullPath().substr(0, uri.getFullPath().size());
    if (!_locations.empty() != 0){
        while (search.length()) {
            for (it = _locations.begin(); it != _locations.end(); it++){
                fullpath = getRoot() + it->first;
                if (fullpath.find(search, _root.length())) {
                    if (fullpath.length() == (getRoot() + search).length()) {
                        ret = *it->second;
                        return ret;
                    }
                }
            }
            removeDir(search);
        }
    }
    return ret;
}


