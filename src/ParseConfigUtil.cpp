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
	std::cout << "cgi_extension:\t\t" << items->getCgiExtension() << "\n";
	std::cout << "upload_path:\t\t" << items->getUploadPath() << "\n";
	std::cout << "upload_enable:\t\t" << items->getUploadEnable() << "\n";
	for (itError = errorCopy.begin(); itError != errorCopy.end(); itError++) {
		std::cout << "error_pages:\t\t" << itError->first << " " << itError->second << "\n";
	}
	std::cout << "client_max_body:\t" << items->getClientMaxBodySize() << "\n";
	for (itLocation = locationsCopy.begin(); itLocation != locationsCopy.end(); itLocation++) {
		std::cout << "Location:\t\t" << itLocation->first << "\n";
	}
    std::cout << "alias:\t\t\t" << items->getAlias() << "\n";
	std::cout << "----------------------" << std::endl;

}
void ParseConfig::fillLocations(){
	std::vector<std::pair<int, Settings* > >::iterator it_server;
	std::vector<std::pair<std::string, Settings* > >::iterator it_location;
	for (it_server = _Config_Vector.begin(); it_server != _Config_Vector.end(); it_server++){
		// std::cout << "Printing Config_vector: " << std::endl;
		// std::cout << "port:\t\t\t" << it_server->first << std::endl;
		if (it_server->first == -1)
			it_server->first = 80;
		if (it_server->second->getAllowMethods() == -1)
			it_server->second->setAllowMethods(0);
		if (it_server->second->getUploadEnable() == -1)
			it_server->second->setUploadEnable(0);
		if (it_server->second->getClientMaxBodySize() == 1000000)
			it_server->second->setClientMaxBodySize(1000000);	
		if (it_server->second->getAutoindex() == "")
			it_server->second->setAutoindex("off");	
		Settings* top = it_server->second;
		std::vector<std::pair<std::string, Settings*> > loc = top->getLocations();
    	for (it_location = loc.begin(); it_location != loc.end(); it_location++) {
			if (it_location->second->getAllowMethods() == -1)
				it_location->second->setAllowMethods(it_server->second->getAllowMethods());
			if (it_location->second->getClientMaxBodySize() == 1)
				it_location->second->setClientMaxBodySize(it_server->second->getClientMaxBodySize());
			if (it_location->second->getAutoindex() == "")
				it_location->second->setAutoindex(it_server->second->getAutoindex());
			if (it_location->second->getRoot() == "")
				it_location->second->setRoot(it_server->second->getRoot());
			if (it_location->second->getCgiPath() == "")
				it_location->second->setCgiPath(it_server->second->getCgiPath());
			if (it_location->second->getCgiExtension() == "")
				it_location->second->setCgiExtension(it_server->second->getCgiExtension());
			if (it_location->second->getUploadPath() == "")
				it_location->second->setUploadPath(it_server->second->getUploadPath());
			if (it_location->second->getUploadEnable() == -1)
				it_location->second->setUploadEnable(it_server->second->getUploadEnable());
			if (it_location->second->getAlias() == "")
				it_location->second->setAlias(it_server->second->getAlias());
			it_location->second->getErrorPages() = it_server->second->getErrorPages();
		
    	}
	}
}

void	ParseConfig::duplicatePort(){
	std::vector<std::pair<int, Settings* > >::iterator it;
	std::vector<std::pair<int, Settings* > >::iterator it_rest;
	for (it = _Config_Vector.begin(); it != _Config_Vector.end(); it++){
		it_rest = it;
		it_rest++;
		for (; it_rest != _Config_Vector.end(); it_rest++){
			if (it->first == it_rest->first)
				ExitString("duplicate port");
		}
		if (it->second->getHost() == "")
			ExitString("No hostname");
	}
}

std::string ParseConfig::findMapInLine(std::map<string, string>& map, std::string word){
	if(map.find(word) != map.end())
		return word;
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
        if (words == wordpos)
			return word;
	}
	return "";
}

void	ParseConfig::ExitString(std::string msg){
	std::cout << "Error: " << msg << std::endl;
	exit(1);
}

void	ParseConfig::VariableToMap(Settings &config, std::string variable, std::string line){
	// std::cout << line << std::endl;
	int temp = 0;
	int semicolons = countCharacter(';', line);
	std::string::size_type pos;
	pos = line.find(";");
	line = line.substr(0, pos);
	
	if (variable == "server" && _serverBracket == 0 && _locationBracket == 0)
	{
		if (semicolons != 0)
			ExitString("Semicolon");
		ParseServer(line);
	}
	else if ((_serverBracket == 1 || _locationBracket == 1) && variable == "listen")
	{
		if (semicolons != 1)
			ExitString("Semicolon");
		ParseListen(line);
		_Config_Vector.back().first = std::stoi(line);
	}
	else if ((_serverBracket == 1 || _locationBracket == 1) && variable == "error_page")
	{
		if (semicolons != 1)
			ExitString("Semicolon");
		ParseErrorPages(line);
		temp = std::stoi(split(line, 2));
		config.addErrorPage(std::make_pair(temp, split(line, 3)));
	}
	else if ((_serverBracket == 1 || _locationBracket == 1) && variable == "client_max_body_size")
	{
		if (semicolons != 1)
			ExitString("Semicolon");
		ParseClientMaxBody(line);
		config.setClientMaxBodySize(static_cast<size_t>(std::stod(line)));
	}
	else if ((_serverBracket == 1 || _locationBracket == 1) && variable == "root")
	{
		if (semicolons != 1)
			ExitString("Semicolon");
		ParseRoot(line);
		config.setRoot(line);
	}
	else if ((_serverBracket == 1 || _locationBracket == 1) && variable == "index")
	{
		if (semicolons != 1)
			ExitString("Semicolon");
		ParseIndex(line);
		config.setIndex(line);
	}
	else if (_serverBracket == 1 && _locationBracket == 0 && variable == "location"){
		if (semicolons != 0)
				ExitString("Semicolon");
		ParseLocation(line);
		_Config_Vector.back().second->getLocations().back().first = line;
	}
	else if ((_serverBracket == 1 || _locationBracket == 1) && variable == "server_name")
	{
		if (semicolons != 1)
			ExitString("Semicolon");
		ParseHost(line);
		config.setHost(line);
	}
	else if ((_serverBracket == 1 || _locationBracket == 1) && variable == "allow_methods")
	{
		if (semicolons != 1)
			ExitString("Semicolon");
		ParseMethods(line);
		config.setAllowMethods(std::stoi(line));
	}
	else if ((_serverBracket == 1 || _locationBracket == 1) && variable == "autoindex")
	{
		if (semicolons != 1)
			ExitString("Semicolon");
		ParseAutoIndex(line);
		line == "off" ? temp = 0 : temp = 1;
		config.setAutoindex(line);
	}
	else if ((_serverBracket == 1 || _locationBracket == 1) && variable == "upload_enable")
	{
		if (semicolons != 1)
			ExitString("Semicolon");
		ParseUploadEnable(line);
		line == "off" ? temp = 0 : temp = 1;
		config.setUploadEnable(temp);
	}
	else if ((_serverBracket == 1 || _locationBracket == 1) && variable == "upload_store")
	{
		if (semicolons != 1)
			ExitString("Semicolon");
		ParseUploadPath(line);
		config.setUploadPath(line);
	}
	else if ((_serverBracket == 1 || _locationBracket == 1) && variable == "cgi_pass")
	{
		if (semicolons != 1)
			ExitString("Semicolon");
		ParseCgiPath(line);
		config.setCgiPath(line);
	}
	else if ((_serverBracket == 1 || _locationBracket == 1) && variable == "cgi_extension")
	{
		if (semicolons != 1)
			ExitString("Semicolon");
		ParseCgiExtension(line);
		config.setCgiExtension(line);
	}
    else if ((_serverBracket == 1 || _locationBracket == 1) && variable == "alias")
    {
        if (semicolons != 1)
            ExitString("Semicolon");
        ParseAlias(line);
        config.setAlias(line);
    }
	else {
		ExitString("Error: VariableToMap line: " + line );
	}
}
