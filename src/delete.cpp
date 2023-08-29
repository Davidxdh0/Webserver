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
	_statusCode = "202";
	// std::string del = "DELETE";
	// if (!checkMethod(del)){
	// 	_statusCode = 405;
	// 	return ;
	// }
	std::cout << "DeletePage = " << path << std::endl;
	if (isDirectory(path)){
		
	}
	if (remove("public/pp/") != 0 ){
		std::cout << "remove went wrong" << std::endl;
		_statusCode = "204";

	}
	sleep(2);
	std::stringstream p;
	p << 	"<html>\n"
			"<body>\n"
			"<h1>File deleted.</h1>\n"
			"</body>\n"
			"</html>\n";
	_body = p.str();
	std::ofstream MyFile("public/pp.txt");
	_statusCode = "200";


}