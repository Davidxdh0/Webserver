#include "../include/ParseConfig.h"
#include "ParseConfig.h"
#include "Settings.h"
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
	size_t	location = 0;
	size_t	server	= 0;
	bool	firstserverblock = 0;
	bool	firstlocationblock = 0;
	int		endline;
	// int		block = 0;
	Settings* temp;
	Settings loc;
	std::vector<pair<int, Settings* > > Config_Vector;

	std::cout << "readconfig" << std::endl;
	while (getline(filestream, line)){
		endline = line.length();
		comment = line.find('#');
		if (comment != std::string::npos)
			endline = comment;
		server = line.substr(0, endline).find("Server");
		if (server != std::string::npos){
			if (firstserverblock == 1){
				// Config_Vector.push_back(std::make_pair(-1, temp));
				// delete temp;
			}
			temp = new Settings;
			Config_Vector.push_back(std::make_pair(-1, temp));
			firstserverblock = 1;
		}
		location = line.substr(0, endline).find("location");
		if (location != std::string::npos){
			if (firstlocationblock == 1){
				// temp->setLocations("location", loc);
			}
			temp = new Settings;
			Config_Vector.push_back(std::make_pair(-1, temp));
			firstlocationblock = 1;
		}
		parseLineConfig(map, line.substr(0, endline), Config_Vector);
		// std::cout << line << std::endl;
	}
	if (firstserverblock == 1){
		Config_Vector.push_back(std::make_pair(-1, temp));
		// delete temp;
	}
	if (firstlocationblock == 1){
		temp->setLocations("location", loc);
	}
	// map.at("listen") = "word";
}

std::string ParseConfig::findMapInLine(std::map<string, string>& map, std::string word){
	
	
	for (size_t p = 0; p < word.length(); p++){
		word[p] = std::toupper(word[p]);
	}
	// std::cout << word << std::endl;
	if(map.find(word) != map.end()){
		// std::cout << "map = " << map[word] << std::endl;
		return word;
	}
	return "";
}



void	ParseConfig::parseLineConfig(std::map<string, string>& map, std::string line, std::vector<pair<int, Settings* > > &config){
	// int	semicolon = line.find(';'); // meerdere in 1 lijn? error.
	// int openBracket = line.find('{');
	
	std::istringstream iss(line);
	std::string word;
	std::string variable = "";
	// std::cout << line << "\n";
	while (iss >> word){
		// std::cout << word << "\n";
		if (variable == "")
			variable = findMapInLine(map, word);
		// std::cout << "map = " << variable << std::endl;
		if (variable == "LISTEN" && !config.empty()){
			if (!config.empty()){
				std::cout << "word: " << word << " varibable: " << variable << std::endl;
				config.back().second->setHost(word);
				// std::cout << "HEKHFD " <<config.back().second->getHost() << std::endl;
			}
		}
		else if (variable == "location" && !config.empty())
			;
		else if (variable != "")
		{
			VariableToMap(*config.back().second, variable);
			// // std::cout << "word: " << word << " varibable: " << variable << std::endl;
			// word = map.at(variable) + " " + word;
			// map.at(variable) = word;
			// std::cout << "map.at(variable): " << map.at(variable) << std::endl;
		}
	}
	config.size();
	// if (config.back().first != 5)
	// 	;
	// std::cout << std::endl;
	// map.at("upload_store") = "wordsdsdsgfdgdfgdf";
	
}

int ParseConfig::ParseConfigFile() {
	std::map<string, string> map;
	std::fstream filestream(_filename);
	
	int p 	= hasAccess(_filename, filestream);
	map 	= initMap();
	readconfig(map, filestream);
	// map.at("upload_store") = "wordsdsdsgfdgdfgdf";
	std::map<std::string, std::string>::iterator it;
	for (it = map.begin(); it != map.end(); ++it) {
        // std::cout << it->first << " - " << it->second << std::endl;
    }

	return p;
}

std::map<string, string> ParseConfig::initMap(){
	std::map<std::string, string> map;
	map["LISTEN"] 				= ""; 
	map["SERVER"]				= "";  
	map["ERROR_PAGE"] 			= "";  
	map["CLIENT_MAX_BODY_SIZE"]	= "";  
	map["ROOT"] 				= ""; 
	map["INDEX"] 				= "";  
	map["LOCATION"]				= "";  
	map["SERVER_NAME"]			= "";  
	map["ALLOW_METHODS"] 		= "";  
	map["AUTOINDEX"]			= "";  
	map["UPLOAD_ENABLE"]		= "";  
	map["UPLOAD_STORE"] 		= "";  
	map["CGI_PASS"] 			= ""; 
	map["CGI_EXTENSION"] 		= "";  
	map["RETURN"] 				= "";  
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

void	ParseConfig::VariableToMap(Settings &config, std::string variable){
	if (variable == "LISTEN")
		config.setHost("Test");
	// else if (variable == "SERVER")
	// 	config.back().second.set
	// else if (variable == "ERROR_PAGE")
	// 	config.back().second.set
	// else if (variable == "CLIENT_MAX_BODY_SIZE")
	// 	config.back().second.set
	// else if (variable == "ROOT")
	// 	config.back().second.set
	// else if (variable == "INDEX")
	// 	config.back().second.set
	// else if (variable == "LOCATION")
	// 	config.back().second.set
	// else if (variable == "SERVER_NAME")
	// 	config.back().second.set
	// else if (variable == "ALLOW_METHODS")
	// 	config.back().second.set
	// else if (variable == "AUTOINDEX")
	// 	config.back().second.set
	// else if (variable == "UPLOAD_ENABLE")
	// 	config.back().second.set
	// else if (variable == "UPLOAD_STORE")
	// 	config.back().second.set
	// else if (variable == "CGI_PASS")
	// 	config.back().second.set
	// else if (variable == "CGI_EXTENSION")
	// 	config.back().second.set
	// else if (variable == "RETURN")
	// 	config.back().second.set
	// else {
	// 	// Handle other cases or provide a default behavior
	// }
}