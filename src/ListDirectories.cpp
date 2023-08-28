#include "Response.h"
#include "utils.h"
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <cstdio>
#include <dirent.h>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <ctime>

void	Response::showDir(){
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir("/Users/dyeboa/Documents/Webserv/public")) == NULL){
		std::cout << "Error: Can't open showDir" << std::endl;
	}
	while ((ent = readdir (dir)) != NULL) {
    	std::cout << "Directory: " << ent->d_name << std::endl;
  	}
  	closedir (dir);
}

/*
Creates an index.html if it can't find an index.html in the desired folder.

todo: 	add checks for autoindex 
		set the correct path.
		Error catching
		add delete button

how to test:
	std::cout << "\n";
	showDir();
	createIndex();
	std::cout << "\n" << std::endl;
*/
void	Response::createIndex(){
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir("/Users/dyeboa/Documents/Webserv/public/upload")) == NULL){
		std::cout << "Error: Can't open showDir" << std::endl;
	}
	while ((ent = readdir (dir)) != NULL) {
		if (!strcmp("index.html", ent->d_name)){
    		std::cout << "Directory: index.html found ->" << ent->d_name << std::endl;
			// return ;
		}
  	}
	closedir(dir);
	std::ofstream file("/Users/dyeboa/Documents/Webserv/public/upload/index.html");
	file << "<!DOCTYPE html>\n"
			"<html>\n"
			"<head>\n"
			"<title>Index of /</title>\n"
			"<style>\n"
			"table {\n"
			"border-collapse: collapse;\n"
			"width: 100%;\n"
			"}\n"
			"th, td {\n"
			"text-align: left;\n"
			"padding: 8px;\n"
			"}\n"
			"</style>\n"
			"</head>\n"
			"<body>\n"
			"<h1>Index of PATH</h1>\n<table>\n"
			"<tr>\n"
            "<th>Name</th>\n"
            "<th>Date</th>\n"
            "<th>Size</th>\n"
			"<th>Delete</th>\n"
        	"</tr>\n";
	if ((dir = opendir("/Users/dyeboa/Documents/Webserv/public")) == NULL){
		std::cout << "Error: Can't open showDir" << std::endl;
	}
	while ((ent = readdir (dir)) != NULL) {	
		struct stat file_info;
		std::string filepath = std::string("/Users/dyeboa/Documents/Webserv/public") + "/" + ent->d_name;
		// std::cout << "Filepath = " << filepath << std::endl;
		file << "<tr>\n";
		if (stat(filepath.c_str(), &file_info) == 0) {
			time_t modified = file_info.st_mtime;
			char time[100];
			std::strftime(time, sizeof(time), "%Y-%m-%d %H:%M:%S", std::localtime(&modified));
			off_t filesize = file_info.st_size;
			file << "<td><a href=" << ent->d_name << ">" << ent->d_name << "</a></td>\n";
			file << "<td>" << time << "</td>\n";
			file << "<td>" << filesize << "</td>\n";
			file << "<td><a href=\"/delete_file?filename=/upload/" << ent->d_name << "\">Click to delete: " << ent->d_name << "</a></td>\n";
		}
		else{
			file << "<td><a href=" << ent->d_name << ">" << ent->d_name << "</a></td>\n";
			file << "<td>" << "Date not found" << "</td>\n";
			file << "<td>" << "Filesize not found" << "</td>\n";
		}
		file << "</tr>\n";
	}
	closedir(dir);
	file << "</table>\n</body>\n"
			"</html>\n";
	file.close();
}

//if path is directory 
bool Response::isDirectory(void) 
{
    struct stat file_dir;
    if (stat(_path.c_str(), &file_dir) == 0)
        return S_ISDIR(file_dir.st_mode);
    return false;
}

bool	Response::findFile(std::string file, std::string path)
{
	DIR *dir;
	struct dirent *ent;
	struct stat regularFile;
	if ((dir = opendir(path.c_str())) == NULL){
		std::cout << "Error: Can't open showDir at findFile" << std::endl;
	}
		while ((ent = readdir (dir)) != NULL) {
			if (!strcmp(file.c_str(), ent->d_name)){
				if (stat(_path.c_str(), &regularFile) == 0){
					std::cout << "Found file ->" << ent->d_name << std::endl;
        			return S_ISREG(regularFile.st_mode);
				}
			}
  		}
	return 0;
}

bool Response::RequestDirectory(void) 
{
	int _locationIndexStatus = 1; // change to configfile variable.
    if (_locationIndexStatus && isDirectory()) // isDirectory eerder callen.
	{ 
		if (findFile("index.html", _path.getFullPath())){
			return 1; // if found show index.html
		}
		else
			return 404; // Error page 404.html -> Not found, (the requested page was not found)
	}
	return 403; // Error page 403.html -> Forbidden (Prohibit access, block directory listing requests and access to admin pages, etc.)
}

std::string	Response::deletePage()
{
	// int pos = _path.getFilename().find("delete_file?filename=");
	// std::string name = _path.getFilename().substr(pos);
	// std::cout << name << std::endl;
	// remove("./upload/index.html");
		
	_path.setFullPath("Webserv/public/upload/index.html");
	std::cout << "HTTP/1.1 303 See Other\r\n";
	std::cout << "Location: index.html\r\n";
	std::cout << "\r\n";
	
	return "index.html";
}