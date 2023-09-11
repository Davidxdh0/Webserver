#ifndef WEBSERV_PARSECONFIG_H
#define WEBSERV_PARSECONFIG_H

#include "Config.h"
#include "ServerControl.h"
#include "Settings.h"
#include "utils.h"
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>

#include <iostream>
#include <map>

class ParseConfig{

public:
    ParseConfig(char* filename);
    ParseConfig(const ParseConfig &src);
	ParseConfig& operator=(const ParseConfig& other);
    ~ParseConfig();

	int 			ParseConfigFile();
	size_t			FindBracket(std::string line);
	void			readconfig(std::map<string, string>& map, std::fstream &filestream);
	std::string 	findMapInLine(std::map<string, string>& map, std::string line);
	void			VariableToMap(Settings &config, std::string variable, std::string line);
	void			PrintSettings(Settings *items);

	void			parseLineConfig(std::map<string, string>& map, std::string line, std::vector<pair<int, Settings* > > &config, int block);
	std::map<string, string> initMap();

    int 	hasAccess(std::string filepath, std::fstream& filestr);
	void	readconfig(std::fstream &filestream);
	
private:
	std::string		_filename;
	Config			_config;
	std::vector<pair<int, Settings* > > _Config_Vector;
	static const int LOCATION = 1;
    static const int SERVER = 2;
	int				_bracket; //0 closed // 1 serveropen // 2 server&locationopen // 3 locationopen

};

#endif