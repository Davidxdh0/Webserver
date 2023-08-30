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
#include<unistd.h>        

void	Response::deletePage(std::string path)
{
	path = "/Users/dyeboa/Documents/Webserv/public/AA";
	_statusCode = "202";
	// std::string del = "DELETE";
	// if (!checkMethod(del)){
	// 	_statusCode = 405;
	// 	std::cout << "Delete method not found" << std::endl;
	// 	return ;
	// }
	if (isDirectory(path)){
		if (path[path.length() - 1] != '/')
			path = path + '/';
		DIR *dir;
		struct dirent *ent;
		if ((dir = opendir(path.c_str())) == NULL){
			std::cout << "remove went wrong" << std::endl;
			_statusCode = "403";
			return ;
		}
		while ((ent = readdir(dir)) != NULL) {
			// if (isDirectory(path + ent->d_name)){
			// 	deletePage(path + ent->d_name);
			// }
			if (std::string(ent->d_name) == "." || std::string(ent->d_name) == "..")
				continue ;
			std::cout << "Deleting " << ent->d_name << std::endl;
			if (remove((path + std::string(ent->d_name)).c_str()) != 0){
				std::cout << "remove went wrong" << std::endl;
				_statusCode = "405";
			}
		}
		closedir(dir);
	}
	std::cout << "Deleting file or directory: " << path << std::endl;
	if (remove(path.c_str()) != 0 ){
		std::cout << "remove went wrong" << std::endl;
		_statusCode = "405";
		return ;
	}
	sleep(2);
	mkdir("public/AA", 0777);
	std::ofstream MyFile("public/AA/first.txt");
	std::ofstream MyFile1("public/AA/second.css");
	std::ofstream MyFile2("public/AA/third.php");
	std::ofstream MyFile3("public/AA/fourth");
	std::ofstream MyFile4("public/AA/fifth.html");
	std::stringstream p;
	p << 	"<html>\n"
			"<body>\n"
			"<h1>File deleted.</h1>\n"
			"</body>\n"
			"</html>\n";
	_body = p.str();
	_statusCode = "200";
	std::cout << "Deletepage succes" << std::endl;
}