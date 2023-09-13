//
// Created by Alle Janse on 8/7/23.
//

#include "Path.h"
#include "iostream"
#include <sstream>
#include <string>

Path::Path() {}

Path::Path(const std::string& str) {
    std::string buf;
    std::istringstream ss(str);

    _full_path = str;
    std::getline(ss, buf, '/');
    while(std::getline(ss, buf, '/')) {
        _tokens.push_back(buf);
    }

    _extension = buf.substr(buf.find_last_of('.') + 1);
    if (_extension.empty())
        _filename = "";
    else
        _filename = buf;
}

Path::Path(const Path &src) {
    *this = src;
}

Path& Path::operator=(const Path& other){
	if (this != &other){
		_full_path = other._full_path;
		_filename = other._filename;
		_extension = other._extension;
	}
	return *this;
}

Path::~Path() {}

bool Path::isDirectory() {
    return _full_path[_full_path.length() - 1] == '/';
}

std::ostream& operator<<(std::ostream& os, const Path& path) {
    os << path.getFullPath();
    return os;
}

std::string operator+(const std::string& str, const Path& path) {
    return str + path.getFullPath();
}