#include "ParseConfig.h"

ParseConfig::ParseConfig(char* filename) : _filename(filename)
{
	_serverBracket = 0;
	_locationBracket = 0;
	// std::cout << "ParseConfig created with an empty construcor" << std::endl;
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

void	ParseConfig::setBrackets(char c, int block){
	// std::cout << "setBrackets " << c << " " << _locationBracket << ":" << _serverBracket <<  std::endl;
	if (c == '{'){
		if (block == SERVER && _serverBracket == 0 && _locationBracket == 0)
			_serverBracket = 1;
		else if (block == LOCATION && _locationBracket == 0 && _serverBracket == 1)
			_locationBracket = 1;
		else{
			std::cout << "setBrackets open fails " << _locationBracket << ":" << _serverBracket <<  std::endl;
			exit(1);
		}
	}
	else if (c == '}'){
		if (block == SERVER &&_serverBracket == 1 && _locationBracket == 0)
			_serverBracket = 0;
		else if (block == LOCATION && _locationBracket == 1 && _serverBracket == 1)
			_locationBracket = 0;
		else if (block == LOCATION && _locationBracket == 0 && _serverBracket == 1)
			_serverBracket = 0;
		else{
			std::cout << "setBrackets close fails " << _locationBracket << ":" << _serverBracket << std::endl;
			exit(1);
		}
	}
			
}
void	ParseConfig::parseLineConfig(std::map<string, string>& map, std::string line, std::vector<pair<int, Settings* > > &config, int block){
	// int	semicolon = line.find(';'); // meerdere in 1 lijn? error.
	// int openBracket = line.find('{');
	
	std::istringstream iss(line);
	std::string word;
	std::string variable = "";

	while (iss >> word){
		// std::cout << word << "\n";	
		if (variable == "")
			variable = findMapInLine(map, word);
		if (variable != "")
		{
			if (block == SERVER)
				VariableToMap(*config.back().second, variable, line);
			else
				VariableToMap(*config.back().second->getLocations().back().second, variable, line);
			while (iss){
				iss >> word;
			}
		}
		else if (word == "}"){
			// std::cout << "parselineconfig setbrackkets" << std::endl; 
			setBrackets('}', block);
		}
		else if (word == "{"){
			// std::cout << "parselineconfig setbrackkets" << std::endl; 
			setBrackets('{', block);
		}
		else if (word == "\n" && word == " " && word == "\t")
			continue;
		else{
			std::cout << "Error: parseLineConfig word: " << word << std::endl; 
			exit(1);
		}
	}
}

void	ParseConfig::readconfig(std::map<string, string>& map, std::fstream &filestream){
	string		line;
	size_t		comment;
	size_t		location = 0;
	size_t		server	= 0;
	int			block = 0; //
	std::string bracket = "";
	int			endline;
	Settings*	temp;
	
	while (getline(filestream, line)){
		endline = line.length();
		comment = line.find('#');
		if (comment != std::string::npos)
			endline = comment;
		//add server
		server = line.substr(0, endline).find("server ");
		if (server != std::string::npos){
			block = SERVER;
			temp = new Settings;
			_Config_Vector.push_back(std::make_pair(-1, temp));
		}
		//add location
		location = line.substr(0, endline).find("location");
		if (location != std::string::npos){
			block = LOCATION;
			Settings* temp = new Settings;
			_Config_Vector.back().second->addLocations(std::make_pair(line, temp));
		}
		parseLineConfig(map, line.substr(0, endline), _Config_Vector, block);
	}
}

std::vector<pair<int, Settings* > > ParseConfig::ParseConfigFile() {
	std::map<string, string> map;
	std::fstream filestream(_filename);
				// std::cout << "ParseConfigFile" << std::endl;
				// std::cout << "HasAccess" << std::endl;
	hasAccess(_filename, filestream);
				// std::cout << "InitMap" << std::endl;
	map	= initMap();
				// std::cout << "Readconfig" << std::endl;
				// std::cout << "----------------------" << std::endl;
	readconfig(map, filestream);
	// PrintVector();
	return _Config_Vector;
}