//
// Created by Alle Janse on 8/7/23.
//

#include "Path.h"
#include "iostream"

Path::Path(): _full_path(), _extension(), _filename() {}

Path::Path(const std::string& root, const std::string& uri) {
    _full_path = root + uri;
    _filename = _full_path.substr(_full_path.find_last_of('/') + 1);
    _extension = _filename.substr(_filename.find_last_of('.') + 1);
    std::cout << "Path: " << _full_path << std::endl;
    std::cout << "Extension: " << _extension << std::endl;
    std::cout << "Filename: " << _filename << std::endl;
}

Path::Path(const Path &src) {
    *this = src;
}

Path::~Path() {}

std::ostream& operator<<(std::ostream& os, const Path& path) {
    os << path.getFullPath();
    return os;
}

std::string operator+(const std::string& str, const Path& path) {
    return str + path.getFullPath();
}