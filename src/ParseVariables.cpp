#include "ParseConfig.h"
#include "Settings.h"



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
	size_t spaces = 0;
	size_t i = 0;
	for (; i < line.size(); ++i) {
		if (line[i] == ' ')
			spaces++;
	}
	if (spaces == i)
		line = "";
	// std::cout << "line:" << line  << " end" << std::endl;
	return ss.str();
	
}
/*

	listen 8000;
	80 is default

*/
bool	ParseConfig::ParseListen(std::string &line)
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
		if (word == "LISTEN")
			continue;
		try {
			port = std::stoi(word);
			if (port < 1 || port > 65000)
				return 0;
			} catch (const std::invalid_argument& e) {
				return false;
			}
		}
	if (words != 2);
		return 0;
	line = to_string(port);
	return 1;
}
/*
	127.0.0.1;
	localhost
*/
bool	ParseConfig::ParseHost(std::string &line)
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
		if (word == "SERVER_NAME")
			continue;
		str = word;
	}
	if (words != 2);
		return 0;
	line = str;
	return 1;
}
/*
	/public
*/
bool	ParseConfig::ParseRoot(std::string line)
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
		if (word == "ROOT")
			continue;
		if (word[0] != '/')
			return 0;
		if (word.size() < 3)
			return 0;
	}
	if (words != 2);
		return 0;
	line = str;
	return 1;
}
/*
	index 		index.html index2.html
	check of bestaat, anders volgende
*/
bool	ParseConfig::ParseIndex(std::string line)
{
	std::istringstream iss(line);
	std::string::size_type pos;
	std::string word;
	std::string str;
	int words = 0;
	int port = 0;
	while(iss >> word){
		if (word == " ")
			continue;
		words++;
		if (word == "INDEX")
			continue;
		pos = word.find(".");
		if (pos != std::string::npos && pos != 0){
			str = word.substr(pos, word.size());
			if (str != "HTML" || str != "PHP")
				return 0;
		}	
	}
	if (words != 2);
		return 0;
	line = str;
	return 1;
}
/*
	allow_methods GET POST DELETE
*/
bool	ParseConfig::ParseMethods(std::string &line)
{
	std::istringstream iss(line);
	std::string word;
	std::string get = "";
	std::string post = "";
	std::string del = "";
	std::string str;
	int words = 0;
	int port = 0;
	while(iss >> word){
		if (word == " ")
			continue;
		words++;
		if (word == "SERVER_NAME")
			continue;
		if (word == "GET" && get == "")
			get = " GET ";
		else if (word == "POST" && post == "")
			post = " POST ";
		else if (word == "DELETE" && del == "")
			del = " DELETE ";
		else
			return 0;
	}
	if (words < 2 || words > 4)
		return 0;
	line = get + post + del;
	return 1;
}
/*
	autoindex on/off
*/
bool	ParseConfig::ParseAutoindex(std::string &line)
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
		if (word == "AUTOINDEX")
			continue;
		if (word != "ON" || word != "OFF")
			return 0;
	}
	if (words != 2);
		return 0;
	line = str;
	return 1;
}
/*
	cgi_pass /Users/dyeboa/.brew/bin/php-cgi
*/
bool	ParseConfig::ParseCgiPath(std::string &line)
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
		if (word == "SERVER_NAME")
			continue;
		str = word;
	}
	if (words != 2);
		return 0;
	line = str;
	return 1;
}

/*
	cgi_extension php
*/
bool	ParseConfig::ParseCgiExtension(std::string &line)
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
		if (word == "SERVER_NAME")
			continue;
		str = word;
	}
	if (words != 2);
		return 0;
	line = str;
	return 1;
}

/*
	upload_store /upload
*/
bool	ParseConfig::ParseUploadPath(std::string &line)
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
		if (word == "UPLOAD_STORE")
			continue;
		if (word.size() < 2 || word[0] != '/')
			return 0;
	}
	if (words != 2);
		return 0;
	line = str;
	return 1;
}

/*
	upload_enable on
*/
bool	ParseConfig::ParseUploadEnable(std::string &line)
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
		if (word == "UPLOAD_ENABLE")
			continue;
		if (word != "ON" || word != "OFF")
			return 0;
	}
	if (words != 2);
		return 0;
	line = word;
	return 1;
}

/*
	error_page 413 error/400.html;
*/
bool	ParseConfig::ParseErrorPages(std::string line)
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
		if (word == "SERVER_NAME")
			continue;
		str = word;
	}
	if (words != 2);
		return 0;
	line = str;
	return 1;
}

/*
	client_max_body_size 100m;
	default file upload size 1m
*/
bool	ParseConfig::ParseClientMaxBody(std::string line)
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
		if (word == "SERVER_NAME")
			continue;
		str = word;
	}
	if (words != 2);
		return 0;
	line = str;
	return 1;
}

/*
	location /upload {
*/
bool	ParseConfig::ParseLocation(std::string line)
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
		if (word == "SERVER_NAME")
			continue;
		str = word;
	}
	if (words != 2);
		return 0;
	line = str;
	return 1;
}

/*
	return 301 /index.html;
	return 301 
	return /index.html;
*/
bool	ParseConfig::ParseReturn(std::string line)
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
		if (word == "SERVER_NAME")
			continue;
		str = word;
	}
	if (words != 2);
		return 0;
	line = str;
	return 1;
}
