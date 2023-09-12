#ifndef WEBSERV_PARSECONFIG_H
#define WEBSERV_PARSECONFIG_H

#include "Config.h"
#include "ServerControl.h"
#include "Settings.h"
#include "utils.h"
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>

#include <iostream>
#include <map>

class ParseConfig{

public:
    ParseConfig(char* filename);
    ParseConfig(const ParseConfig &src);
	ParseConfig& operator=(const ParseConfig& other);
    ~ParseConfig();

	std::vector<pair<int, Settings* > >	ParseConfigFile();
	size_t			FindBracket(std::string line);
	void			readconfig(std::map<string, string>& map, std::fstream &filestream);
	std::string 	findMapInLine(std::map<string, string>& map, std::string line);
	void			VariableToMap(Settings &config, std::string variable, std::string line);
	void			PrintSettings(Settings *items);

	void			parseLineConfig(std::map<string, string>& map, std::string line, std::vector<pair<int, Settings* > > &config, int block);
	std::map<string, string> initMap();

    int 	hasAccess(std::string filepath, std::fstream& filestr);
	void	readconfig(std::fstream &filestream);
	
	std::string ParseLine(std::string line);
	void ParseListen(std::string &line);
	void ParseServer(std::string &line);
    void ParseHost(std::string &line);
    void ParseRoot(std::string &line);
    void ParseIndex(std::string &line);
    void ParseMethods(std::string &line);
    void ParseAutoIndex(std::string &line);
    void ParseCgiPath(std::string &line);
    void ParseCgiExtension(std::string &line);
    void ParseUploadPath(std::string &line);
    void ParseUploadEnable(std::string &line);
    void ParseErrorPages(std::string &line);
    void ParseClientMaxBody(std::string &line);
    void ParseLocation(std::string &line);
    void ParseReturn(std::string line);
	void ParseAlias(std::string &line);

	void	PrintVector(std::vector<pair<int, Settings* > > Config_Vector);
	void	setBrackets(char c, int block);
	void	substrSemicolon(std::string &line);
	int		countCharacter(char c, std::string line);
	std::string	split(std::string line, int wordpos);
private:
	std::string		_filename;
	Config			_config;
	std::vector<pair<int, Settings* > > _Config_Vector;
	
	static const int LOCATION = 1;
    static const int SERVER = 2;
	int				server;
	int				_serverBracket; //0 closed // 1 open 
	int				_locationBracket;

};

#endif