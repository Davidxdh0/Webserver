#include "../include/ParseConfig.h"
#include "ParseConfig.h"
#include <iostream>

ParseConfig::ParseConfig(char* filename) : _filename(filename)
{
	std::cout << "ParseConfig created with an empty construcor" << std::endl;
}

ParseConfig::~ParseConfig()
{
	std::cout << "ParseConfig deconstructed" << std::endl;
}

ParseConfig::ParseConfig(const ParseConfig &other) : _filename(other._filename), _config(other._config)
{
	std::cout << "ParseConfig created by copy" << std::endl;
}

ParseConfig &ParseConfig::operator=(const ParseConfig &other) 
{
	std::cout << "ParseConfig created by assertion" << std::endl;
	if (this != &other){
		_filename = other._filename;
		_config = other._config;
	}
	return (*this);
}

int ParseConfig::hasAccess(std::string filepath, std::fstream& filestr){
	std::string path = "./" + filepath;
	if (access (path.c_str(), F_OK) != 0){
		std::cout << "ParseConfig doesn't exist" << std::endl;
		return 404; // doesnt exist
	}
	if (filestr.is_open()){
		std::cout << "ParseConfig already open" << std::endl;
		return 403; //already open
	}
	if (access (path.c_str(), R_OK) != 0){
		std::cout << "ParseConfig no read access" << std::endl;
		return 403; // no read access
	}
	filestr.open(path);
	if (!filestr.is_open()){
		std::cout << "ParseConfig not opened" << std::endl;
		return 404; //not opened
	}
	if (filestr.peek() == std::ifstream::traits_type::eof()) {
		std::cout << "ParseConfig empty file" << std::endl;
		return 404; // empty file
	}
	return 200;
}
//check !empty line
//check op '#' ';' einde line of 
//dan zoeken wat eerste woord, $variable/expand, open/close block { } is.
//per open/close block een location. nested locations mag?
//dus per open block een config/settings block. - meerdere ports per settings
void	ParseConfig::readconfig(std::fstream &filestream){
	string	line;
	// int i = 0;
	std::cout << "readconfig" << std::endl;
	while (getline(filestream, line)){
		std::cout << line << std::endl;
	}
}

// void	Config::initialiseConfig(int argc, char *argv[]){

// 	// setWithFile(const char *fileLocation);
// 	// createConfig(config_file);
// }
int ParseConfig::ParseConfigFile() {
	std::fstream filestream(_filename);
	int p = hasAccess(_filename, filestream);
	readconfig(filestream);
	std::cout << p << std::endl;

	return p;
}

