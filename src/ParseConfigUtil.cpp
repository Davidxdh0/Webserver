#include "ParseConfig.h"
#include "Settings.h"
void	ParseConfig::PrintVector(std::vector<pair<int, Settings* > > Config_Vector )
{	
	int		amountLocation = 0;
	int		amountServer = 0;
	std::vector<std::pair<int, Settings* > >::iterator it;
	std::vector<std::pair<std::string, Settings* > >::iterator it_location;
	amountServer = 0;
	for (it = Config_Vector.begin(); it != Config_Vector.end(); it++){
		std::cout << "Printing Config_vector: " << ++amountServer<< std::endl;
		std::cout << "port:\t\t\t" << it->first << std::endl;
		Settings* top = it->second;
		PrintSettings(top);
		std::vector<std::pair<std::string, Settings*> > locationsCopy = top->getLocations();
		amountLocation = 0;
    	for (it_location = locationsCopy.begin(); it_location != locationsCopy.end(); it_location++) {
			std::cout << "Location " << ++amountLocation << ": " << it_location->first << std::endl;
    	    PrintSettings(it_location->second);
    	}
	}
}
void	ParseConfig::PrintSettings(Settings *items)
{
	std::vector<std::pair<int, std::string> > errorCopy = items->getErrorPages();
	std::vector<std::pair<int, std::string> >::iterator itError;
	std::vector<std::pair<std::string, Settings*> > locationsCopy = items->getLocations();
	std::vector<std::pair<std::string, Settings* > >::iterator itLocation;

	std::cout << "Settings:" << std::endl;
	std::cout << "host:\t\t\t" << items->getHost() << "\n";
	std::cout << "root:\t\t\t" << items->getRoot() << "\n";
	std::cout << "index:\t\t\t" << items->getIndex() << "\n";
	std::cout << "methods:\t\t" << items->getAllowMethods() << "\n";
	std::cout << "autoindex:\t\t" << items->getAutoindex() << "\n";
	std::cout << "cgi path:\t\t" << items->getCgiPath() << "\n";
	std::cout << "cgi_extension:\t" << items->getCgiExtension() << "\n";
	std::cout << "upload_path:\t\t" << items->getUploadPath() << "\n";
	std::cout << "upload_enable:\t\t" << items->getUploadPath() << "\n";
	for (itError = errorCopy.begin(); itError != errorCopy.end(); itError++) {
		std::cout << "error_pages:\t\t" << itError->first << " " << itError->second << "\n";
	}
	std::cout << "client_max_body:\t" << items->getClientMaxBodySize() << "\n";
	for (itLocation = locationsCopy.begin(); itLocation != locationsCopy.end(); itLocation++) {
		std::cout << "Location:\t\t" << itLocation->first << "\n";
	}
	std::cout << "return:\t\t\t" << items->getReturn() << "\n";
	std::cout << "----------------------" << std::endl;

}

std::string ParseConfig::findMapInLine(std::map<string, string>& map, std::string word){
	if(map.find(word) != map.end()){
		return word;
	}
	return "";
}

std::map<string, string> ParseConfig::initMap(){
	std::map<std::string, string> map;
	map["listen"] 				= ""; 
	map["server"]				= "";  
	map["error_page"] 			= "";  
	map["client_max_body_size"]	= "";  
	map["root"] 				= ""; 
	map["index"] 				= "";  
	map["location"]				= "";  
	map["server_name"]			= "";  
	map["allow_methods"] 		= "";  
	map["autoindex"]			= "";  
	map["upload_enable"]		= "";  
	map["upload_store"] 		= "";  
	map["cgi_pass"] 			= ""; 
	map["cgi_extension"] 		= "";  
	map["return"] 				= "";
	map["alias"] 				= "";
	return map;
}

std::string	ParseConfig::split(std::string line, int wordpos){
	std::stringstream ss(line);
	std::string word;
	int words = 0;

	while (ss >> word) {
		if (word == " ")
			break;
        words++;
        if (words == wordpos) {
			return word;
		}
	}
	return "";
}

void	ParseConfig::VariableToMap(Settings &config, std::string variable, std::string line){
	line = ParseLine(line);
	// std::cout << line << std::endl;
	int temp = 0;
	int semicolons = countCharacter(';', line);
	std::string::size_type pos;
	pos = line.find(";");
	line = line.substr(0, pos);
	
	if (variable == "listen")
	{
		if (semicolons != 1)
			exit(1);
		ParseListen(line);
		_Config_Vector.back().first = std::stoi(line);
	}
	else if (variable == "server")
	{
		if (semicolons != 0)
			exit(1);
		ParseServer(line);
		// config.setHost(line);
	}
	else if (variable == "error_page")
	{
		if (semicolons != 1)
			exit(1);
		ParseErrorPages(line);
		temp = std::stoi(split(line, 2));
		config.addErrorPage(std::make_pair(temp, split(line, 3)));
	}
	else if (variable == "client_max_body_size")
	{
		if (semicolons != 1)
			exit(1);
		ParseClientMaxBody(line);
		config.setClientMaxBodySize(static_cast<size_t>(std::stod(line)));
	}
	else if (variable == "root")
	{
		if (semicolons != 1)
			exit(1);
		ParseRoot(line);
		config.setRoot(line);
	}
	else if (variable == "index")
	{
		if (semicolons != 1)
			exit(1);
		ParseIndex(line);
		config.setIndex(line);
	}
	else if (variable == "location"){
		if (semicolons != 0)
			exit(1);
		ParseLocation(line);
		_Config_Vector.back().second->getLocations().back().first = line;
	}
	else if (variable == "server_name")
	{
		if (semicolons != 1)
			exit(1);
		ParseHost(line);
		config.setHost(line);
	}
	else if (variable == "allow_methods")
	{
		if (semicolons != 1)
			exit(1);
		ParseMethods(line);
		config.setAllowMethods(std::stoi(line));
	}
	else if (variable == "autoindex")
	{
		if (semicolons != 1)
			exit(1);
		ParseAutoIndex(line);
		line == "off" ? temp = 0 : temp = 1;
		config.setAutoindex(line);
	}
	else if (variable == "upload_enable")
	{
		if (semicolons != 1)
			exit(1);
		ParseUploadEnable(line);
		line == "off" ? temp = 0 : temp = 1;
		config.setUploadEnable(temp);
	}
	else if (variable == "upload_store")
	{
		if (semicolons != 1)
			exit(1);
		ParseUploadPath(line);
		config.setUploadPath(line);
	}
	else if (variable == "cgi_pass")
	{
		if (semicolons != 1)
			exit(1);
		ParseCgiPath(line);
		config.setCgiPath(line);
	}
	else if (variable == "cgi_extension")
	{
		if (semicolons != 1)
			exit(1);
		ParseCgiExtension(line);
		config.setCgiExtension(line);
	}
	else if (variable == "return")
	{
		if (semicolons != 1)
			exit(1);
		ParseReturn(line);
		std::string three = split(line, 3);
		if (three != "")
			line = split(line, 2) + " " + split(line, 3);
		else
			line = split(line, 2);
		config.setReturn(line);
	}
	else {
		std::cout << "Not in map: " << variable << " line: " << line << std::endl;
	}
}
