#include "../include/Config.h"
// #include "Config.hpp"
#include <iostream>
#include <fstream>

Config::Config() : _port()
{
	std::cout << "Config created with an empty construcor" << std::endl;
}

Config::Config(const char *fileLocation)
{
	try
	{
		this->setWithFile(fileLocation);
	}
	catch (Config::FileNotRetrievedException &e)
	{
		std::cerr << e.what() << std::endl;
	}
}

Config::~Config(void)
{
	std::cout << "Config deconstructed" << std::endl;
}

Config::Config(const Config &other) : _port()
{
	std::cout << "Config created by copy" << std::endl;
	*this = other;
}

Config::Config(int port)
{
    std::cout << "Config created with port" << std::endl;
    this->_port = port;
}

Config &Config::operator=(const Config &other)
{
	std::cout << "Config created by assertion" << std::endl;

	if (this != &other)
	{
        _port = other._port;
	}

	return (*this);
}

// ---------------------- Config member functions ------------------------------

//void Config::setWithFile(const char *fileLocation)
//{
//	if (!this->_configFile.is_open())
//	{
//		this->_configFile.close();
//	}
//
//	this->_configFile.open(fileLocation);
//
//	if (!this->_configFile.is_open())
//	{
//		throw Config::FileNotFoundException();
//	}
//
//	if (this->_configFile.fail())
//	{
//		throw Config::FileNotFoundException();
//	}
//
//	this->_fileLocation = fileLocation;
//}


// void	createConfig(){
	
// 	map<std::string, map<std::string, int> > _mapConfig;
// 	string	line;
// 	int i = 0;
// 	while (getline(config_file, line) && i < 6){
// 		//get first 6 line from config - defaultConfig
// 		// hoe gaan andere configfiles?
// 	}
// 	while (getline(config_file, line)){
// 		//get for each location variables - int = enum
// 	}
// 	;
// }

// void	Config::initialiseConfig(int argc, char *argv[]){

// 	// setWithFile(const char *fileLocation);
// 	// createConfig(config_file);
// }
// ---------------------- Config Exception Functions ------------------------

const char *Config::FileNotRetrievedException::what() const throw()
{
	return ("Error finding or opening the file");
}

const char *Config::FileIncorrectFormatException::what() const throw()
{
	return ("Error parsing the file");
};
