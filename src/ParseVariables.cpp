#include "ParseConfig.h"
#include "Settings.h"

int	ParseConfig::isInt(const std::string& input){
	int i = 0;

	while (input[i] != '\0' && std::isdigit(input[i]))
		i++;
	if (input[i] == '\0')
		return 0;
	return i;
}

std::string	ParseConfig::ParseLine(std::string line){
	std::string::iterator it;
	std::string::iterator it_rhs;
	size_t i_rhs;
	std::stringstream ss;
	char c;
	for(it = line.begin(); it != line.end(); it++){
		char p = *it;
		if (p == '\t')
			ss << ' ';
		else
			ss << p;
	}
	line = ss.str();
	ss.str("");
	for (size_t i = 0; i < line.size(); ++i) {
		i_rhs = i + 1;
		if (i_rhs != line.size()){
			c = line[i];
			char c_rhs = line[i_rhs];
			if (c == ' ' && c_rhs == ' '){
				while(i != line.size() && c_rhs == ' '){
					i_rhs++;
					c_rhs = line[i_rhs];
				}
				i_rhs--;
				ss << ' ';
				i = i_rhs;
			}
			else
				ss << c;
		}
		else
			ss << line[i];
	}	
	line = ss.str();
	return ss.str();
}

int		ParseConfig::countCharacter(char c, std::string line){
	int count = 0;
	for(size_t i = 0; i < line.size(); i++){
		if (line[i] == c)
			count++;
	}
	return count;
}

void ParseConfig::substrSemicolon(std::string &line){
	std::string::size_type pos;
	pos = line.find(";");
	if (pos != std::string::npos)
		line = line.substr(0, pos);
}

void	ParseConfig::ParseServer(std::string &line)
{
	std::istringstream iss(line);
	std::string word;
	std::string str;
	int words = 0;
	while(iss >> word){
		if (word == " ")
			continue;
		if (word == "{")
			setBrackets('{', SERVER);
		if (word == "}")
			setBrackets('}', SERVER);
		words++;
		if (word == "server")
			continue;
		str = word;
	}
	if (words != 2)
		ExitString("ParseServer words != 2");
	line = str;
}

/*
	listen 8000;
	80 is default
*/

void	ParseConfig::ParseListen(std::string &line)
{	
	std::istringstream iss(line);
	std::string word;
	std::string str;
	int words = 0;
	int port = 0;
	while(iss >> word){
		if (word == " ")
			continue;
		words++;
		if (word == "listen")
			continue;
		else{
			try {
				port = std::stoi(word);
				if (port < 1 || port > 65000){
					ExitString("ParseListen port out of range < 1 || > 65000");
				}
				} catch (const std::invalid_argument& e) {
					ExitString("ParseListen port is not an int");
				}
		}
	}
	if (words > 2)
		ExitString("ParseListen words > 2");
	line = to_string(port);
	if (words == 1)
		line = "80";
}
/*
	127.0.0.1;
	localhost
*/
void	ParseConfig::ParseHost(std::string &line)
{
	std::istringstream iss(line);
	std::string word;
	std::string str;
	int words = 0;
	while(iss >> word){
		if (word == " ")
			continue;
		words++;
		if (word == "server_name")
			continue;
		str = word;
	}
	if (words != 2)
		ExitString("ParseHost words != 2");
	line = str;
}

bool isRootDirectory(std::string dirpath) 
{
    struct stat file_dir;
    if (stat(dirpath.c_str(), &file_dir) == 0)
        return S_ISDIR(file_dir.st_mode);
    return false;
}

/*
	/public
*/
void	ParseConfig::ParseRoot(std::string &line)
{
	std::istringstream iss(line);
	std::string word;
	std::string str;
	int words = 0;

	while(iss >> word){
		if (word == " ")
			continue;
		words++;
		if (word == "root")
			continue;
		if (word[0] != '/')
			exit(1);
		if (word.size() < 3)
			exit(1);
		if (!isRootDirectory(word)){
			str = "ParseRoot: " + word;
			ExitString(str);
		}
		str = word;
	}
	if (words != 2)
		exit(1);
	line = str;
}
/*
	index 		index.html index2.html
	check of bestaat, anders volgende
*/
void	ParseConfig::ParseIndex(std::string &line)
{
	std::istringstream iss(line);
	std::string::size_type pos;
	std::string word;
	std::string str;
	int words = 0;
	while(iss >> word){
		if (word == " ")
			continue;
		words++;
		if (word == "index")
			continue;
		pos = word.find(".");
		if (pos != std::string::npos && pos != 0){
			str = word.substr(pos + 1, word.size());
			if (str != "html" && str != "php"){
				ExitString("Index not .html or .php");
			}
		}	
	}
	if (words != 2)
		ExitString("ParseIndex words != 2");
	line = word;
}
/*
	allow_methods GET POST DELETE
*/
void	ParseConfig::ParseMethods(std::string &line)
{
	std::istringstream iss(line);
	std::string word;
	int get = 0;
	int post = 0;
	int del = 0;
	std::string str;
	int words = 0;
	while(iss >> word){
		if (word == " ")
			continue;
		words++;
		if (word == "allow_methods")
			continue;
		else if (word == "GET" && get == 0)
			get = 1;
		else if (word == "POST" && post == 0)
			post = 2;
		else if (word == "DELETE" && del == 0)
			del = 4;
		else{
			str = "ParseMethods " + word;
			ExitString(str);;
		}
	}
	if (words < 2 || words > 4)
		ExitString("ParseMethods words > 4");
	line = to_string(get + post + del);
}
/*
	autoindex on/off
*/
void	ParseConfig::ParseAutoIndex(std::string &line)
{
	std::istringstream iss(line);
	std::string word;
	std::string str;
	int words = 0;
	while(iss >> word){
		if (word == " ")
			continue;
		words++;
		if (word == "autoindex")
			continue;
		if (word != "on" && word != "off")
			ExitString("ParseIndex != on/off");
	}
	if (words != 2)
		ExitString("ParseAutoindex words != 2");
	line = word;
}
/*
	cgi_pass /Users/dyeboa/.brew/bin/php-cgi
*/
void	ParseConfig::ParseCgiPath(std::string &line)
{
	std::istringstream iss(line);
	std::string word;
	std::string str;
	int words = 0;
	while(iss >> word){
		if (word == " ")
			continue;
		words++;
		if (word == "cgi_pass")
			continue;
	}
	std::fstream filestream(word);
	hasAccess(word, filestream)
	filestream.close();
	if (words != 2 )
		ExitString("ParseCgiPath != 2");
	line = word;
}

/*
	cgi_extension php
*/
void	ParseConfig::ParseCgiExtension(std::string &line)
{
	std::istringstream iss(line);
	std::string word;
	std::string str;
	int words = 0;
	while(iss >> word){
		if (word == " ")
			continue;
		words++;
		if (word == "cgi_extension")
			continue;
		if (word != "php")
			exit(1);
		str = word;
	}
	if (words != 2)
		ExitString("ParseCgiExt != 2");
	line = str;
}

/*
	upload_store /upload
*/
void	ParseConfig::ParseUploadPath(std::string &line)
{
	std::istringstream iss(line);
	std::string word;
	std::string str;
	int words = 0;
	while(iss >> word){
		if (word == " ")
			continue;
		words++;
		if (word == "upload_store")
			continue;
		else if (word.size() < 2 && word[0] != '/')
			ExitString("ParseUploadPath");
		str = word;
	}
	if (words != 2)
		ExitString("ParseUploadPath != 2");
	line = str;
}

/*
	upload_enable on
*/
void	ParseConfig::ParseUploadEnable(std::string &line)
{
	std::istringstream iss(line);
	std::string word;
	std::string str;
	int words = 0;
	while(iss >> word){
		if (word == " ")
			continue;
		words++;
		if (word == "upload_enable")
			continue;
		if (word != "on" && word != "off")
			ExitString("ParseUploadEnable != on/off");
	}
	if (words != 2)
		ExitString("ParseUploadEnable != 2");
	line = word;
}

/*
	error_page 413 error/400.html;
*/
void	ParseConfig::ParseErrorPages(std::string &line)
{
	std::istringstream iss(line);
	std::string word;
	std::string str;
	int words = 0;
	while(iss >> word){
		if (word == " ")
			continue;
		words++;
		if (word == "error_page")
			continue;
	}
	if (words != 3)
		ExitString("ParseErrorPages != 3");
}

/*
	client_max_body_size 100m;
	client_max_body_size 100k;
	client_max_body_size 100g;
	default file upload size 1m
	
*/
void	ParseConfig::ParseClientMaxBody(std::string &line)
{
	std::istringstream iss(line);
	std::string word;
	std::string str;
	int firstChar = 0;
	int words = 0;
	double bytes = 0;
    double type = 1;
	while(iss >> word){
		if (word == " ")
			continue;
		words++;
		if (word == "client_max_body_size")
			continue;
		else{
			firstChar = isInt(word);
			if (firstChar > 0 && (word.size() == static_cast<size_t>(firstChar) + 1)){
				if (toupper(word[firstChar]) == 'M')
				type = 1000000;
				else if (toupper(word[firstChar]) == 'K')
					type = 1000;
				else if (toupper(word[firstChar]) == 'G')
					type = 100000000;
				else if (isdigit(word[firstChar]))
					word += "X";
				else
					ExitString("ParseClientMaxBody size");
				word = word.substr(0, word.size() - 1);
			}
			try {
				bytes = std::stoi(word);
				bytes *= type;
				line = to_string(static_cast<size_t>(bytes));
			} catch (const std::exception& e){
				ExitString("ParseClientMaxBody stoi");
			}
		}
	}
	if (words != 1 && words != 2)
		ExitString("ParseClientMaxBody not 1 or 2");
	if (words == 1)
		line = "1000000";
}

/*
	location /upload {
*/
void	ParseConfig::ParseLocation(std::string &line)
{
	std::istringstream iss(line);
	std::string word;
	std::string str;
	int words = 0;
	while(iss >> word){
		if (word == " ")
			break;
		if (word == "{"){
			setBrackets('{', LOCATION);
			break;
		}
		if (word == "}"){
			setBrackets('}', LOCATION);
			break;
		}
		words++;
		if (word == "location")
			continue;
		if (words == 1 && !std::isalpha(word[0]) && word[0] != '/')
			ExitString("ParseLocation Fails");
		str = word;
	}
	if (words != 2)
		ExitString("ParseLocation != 2");
	line = str;
}

void	ParseConfig::ParseAlias(std::string &line){
	std::istringstream iss(line);
	std::string word;
	std::string str;
	int words = 0;
	while(iss >> word){
		if (word == " ")
			continue;
		words++;
		if (word == "alias")
			continue;
        else
            if (word[0] != '/')
                ExitString("ParseAlias");
	}
	if (words != 2)
		ExitString("ParseAlias != 2");
	line = word;
}
