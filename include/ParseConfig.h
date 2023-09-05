#ifndef WEBSERV_PARSECONFIG_H
#define WEBSERV_PARSECONFIG_H

#include "Config.h"
#include "ServerControl.h"
#include "utils.h"
#include <unistd.h>
#include <fstream>


#include <iostream>
#include <map>

class ParseConfig{

public:
    ParseConfig(char* filename);
    ParseConfig(const ParseConfig &src);
	ParseConfig& operator=(const ParseConfig& other);
    ~ParseConfig();

	int 	ParseConfigFile();
    int 	hasAccess(std::string filepath, std::fstream& filestr);
	void	readconfig(std::fstream &filestream);
	
private:
	std::string		_filename;
	Config			_config;
};

#endif