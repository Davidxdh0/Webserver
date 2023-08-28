//
// Created by Alle Janse on 8/7/23.
//

#ifndef WEBSERV_PATH_H
#define WEBSERV_PATH_H


#include <string>

class Path {

public:
    Path();
    Path(const std::string& root, const std::string& uri);
    Path(const Path &src);
    ~Path();

	void   			setFullPath(std::string newPath) { _full_path = newPath; };
    std::string     getFullPath() const { return _full_path; };
    std::string     getExtension() { return _extension; };
    std::string     getFilename() { return _filename; };
    const char*     c_str() const { return _full_path.c_str(); };
private:

    std::string     _full_path;
    std::string     _extension;
    std::string     _filename;


};

std::ostream& operator<<(std::ostream& os, const Path& path);

std::string operator+(const std::string& str, const Path& path);

#endif //WEBSERV_PATH_H
