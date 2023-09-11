#include "ParseConfig.h"
#include "Settings.h"

// find bracket en check 
size_t ParseConfig::FindBracket(std::string line)
{

}


// while (errorCopy.empty()) {
// 	std::cout << "error_pages:\t\t" << items->getErrorPageInt();
// 	std::cout << " " << items->getErrorPageString() << "\n";
// 	items->getErrorPages().pop_back();
// }
// while (copyLocation.empty()) {
// 	std::cout << "locations:\t\t" << copyLocation.back << "\n";
// }
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


void	ParseConfig::VariableToMap(Settings &config, std::string variable, std::string line){
	size_t firstNonWhitespace = line.find_first_not_of(" \t");
    if (firstNonWhitespace != std::string::npos)
        line = line.substr(firstNonWhitespace);

	if (variable == "LISTEN")
		config.setHost(line);
	else if (variable == "SERVER")
		config.setHost(line);
	else if (variable == "ERROR_PAGE")
		config.addErrorPage(std::make_pair(-1, line));
	else if (variable == "CLIENT_MAX_BODY_SIZE")
		config.setClientMaxBodySize(5000);
	else if (variable == "ROOT")
		config.setRoot(line);
	else if (variable == "INDEX")
		config.setIndex(line);
	else if (variable == "LOCATION"){
		// Settings* settingsPtr = new Settings;
		// std::vector<std::pair<std::string, Settings*> > temp;
		// temp.push_back(std::make_pair(line, settingsPtr));
		// config.setLocations(temp);
		;// config.addLocations(std::make_pair(line, settingsPtr));
	}
	else if (variable == "SERVER_NAME")
		config.setHost(line);
	else if (variable == "ALLOW_METHODS")
		config.setAllowMethods(-1);
	else if (variable == "AUTOINDEX")
		config.setAutoindex(line);
	else if (variable == "UPLOAD_ENABLE")
		config.setUploadPath(line);
	else if (variable == "UPLOAD_STORE")
		config.setUploadPath(line);
	else if (variable == "CGI_PASS")
		config.setCgiPath(line);
	else if (variable == "CGI_EXTENSION")
		config.setCgiExtension(line);
	else if (variable == "RETURN")
		config.setReturn(line);
	else {
		std::cout << "Not in map: " << variable << std::endl;
	}
	// std::cout << "Variable Set: " << variable << " line: " << line << std::endl;
}
