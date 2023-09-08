#include "../include/ParseConfig.h"
#include "ParseConfig.h"
#include <iostream>
#include <sstream>
#include <map>

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
void	ParseConfig::readconfig(std::map<string, string>& map, std::fstream &filestream){
	string	line;
	size_t	comment;
	int		endline;
	std::cout << "readconfig" << std::endl;
	while (getline(filestream, line)){
		endline = line.length();
		comment = line.find('#');
		if (comment != std::string::npos)
			endline = comment;
		parseLineConfig(map, line.substr(0, endline));
		// std::cout << line << std::endl;
	}
}

std::string ParseConfig::findMapInLine(std::map<string, string>& map, std::string word){
	
	if(map.find(word) != map.end())
		return map[word];
	return "";
}


void	ParseConfig::parseLineConfig(std::map<string, string>& map, std::string line){
	// int	semicolon = line.find(';'); // meerdere in 1 lijn? error.
	// int openBracket = line.find('{');
	
	std::istringstream iss(line);
	std::string word;
	std::string variable = "";
	while (iss){
		iss >> word;
		if (variable == "")
			variable = findMapInLine(map, word);
		if (variable == "Server")
			;
		else if (variable == "location")
			;
		else if (variable != "")
		{
			word = map.at(variable) + " " + word;
			map.at(variable) = word;
		}
	}
}

// void	Config::initialiseConfig(int argc, char *argv[]){

// 	// setWithFile(const char *fileLocation);
// 	// createConfig(config_file);
// }
int ParseConfig::ParseConfigFile() {
	std::map<string, string> map;
	std::fstream filestream(_filename);
	int p 	= hasAccess(_filename, filestream);
	map 	= initMap();
	readconfig(map, filestream);

	std::map<std::string, std::string>::iterator it;
	for (it = map.begin(); it != map.end(); ++it) {
        std::cout << it->first << " - " << it->second << std::endl;
    }

	return p;
}

std::map<string, string> ParseConfig::initMap(){
	std::map<std::string, string> map;
	map["listen"] 				= ""; 
	map["Server"]				= "";  
	map["error_page"] 			= "";  
	map["client_max_body_size"]	= "";  
	map["root"] 				= ""; 
	map["index"] 				= "";  
	map["location"]				= "";  
	map["server_name"]			= "";  
	map["allow_methods"] 		= "";  
	map["autoindex"]			= "";  
	map["upload_store"] 		= "";  
	map["cgi_pass"] 			= ""; 
	map["cgi_extension"] 		= "";  
	map["return"] 				= "";  
	return map;
}



// Port = 0,
// Servername,
// Errorpage,
// Client_max_body,
// Root,
// Index,
// Location,
// Name,
// Methods,
// Autoindex,
// Upload,
// Loc_handling

