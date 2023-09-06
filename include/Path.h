//
// Created by Alle Janse on 8/7/23.
//

#ifndef WEBSERV_PATH_H
#define WEBSERV_PATH_H


#include <string>
#include <vector>

class Path {

public:
    Path();
    Path(const std::string& uri);
    Path(const Path &src);
	Path& operator=(const Path& other);
    ~Path();

	void   			setFullPath(std::string newPath) { _full_path = newPath; };
    std::string     getFullPath() const { return _full_path; };
    std::string     getExtension() { return _extension; };
    std::string     getFilename() { return _filename; };
    const char*     c_str() const { return _full_path.c_str(); };
private:

    std::string                 _full_path;
    std::vector<std::string>    _tokens;
    std::string                 _extension;
    std::string                 _filename;


};

std::ostream& operator<<(std::ostream& os, const Path& path);

std::string operator+(const std::string& str, const Path& path);

#endif //WEBSERV_PATH_H
