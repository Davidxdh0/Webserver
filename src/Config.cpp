#include "../include/Config.h"
// #include "Config.hpp"
#include <iostream>
#include <ifstream>

Config::Config() : port()
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

Config::Config(const Config &other) : port()
{
	std::cout << "Config created by copy" << std::endl;
	*this = other;
}

Config::Config(int port)
{
    std::cout << "Config created with port" << std::endl;
    this->port = port;
}

Config &Config::operator=(const Config &other)
{
	std::cout << "Config created by assertion" << std::endl;

	if (this != &other)
	{
        port = other.port;
	}

	return (*this);
}

// ---------------------- Config member functions ------------------------------

void Config::setWithFile(const char *fileLocation)
{
	if (!this->_configFile.is_open())
	{
		this->_configFile.close();
	}

	this->_configFile.open(fileLocation);

	if (!this->_configFile.is_open())
	{
		throw Config::FileNotRetrievedException();
	}

	if (this->_configFile.fail())
	{
		throw Config::FileNotRetrievedException();
	}

	this->_fileLocation = fileLocation;
}



void	Config::openConfig(ifstream& config_file, int argc, char *argv[]){
	try {
		if (argc == 2){
			if (!config_file.open(argv[1]))
				throw std::exception("Error: Can't open configfile.")
		}
		else{
			if (!config_file.open(this->_configFile))
				throw std::exception("Error: Can't open configfile.")
		}
	}
}

void	createConfig(ifstream& config_file){
	
	map<std::string, map<std::string, int> > _mapConfig;
	string	line;
	int i = 0;
	while (getline(config_file, line) && i < 6){
		//get first 6 line from config - defaultConfig
		// hoe gaan andere configfiles?
	}
	while (getline(config_file, line)){
		//get for each location variables - int = enum
	}
}

void	Config::initialiseConfig(int argc, char *argv[]){
	ifstream	config_file;
	
	openConfig(config_file, argc, argv);
	createConfig(config_file);
}
// ---------------------- Config Exception Functions ------------------------

const char *Config::FileNotRetrievedException::what() const throw()
{
	return ("Error finding or opening the file");
}

const char *Config::FileIncorrectFormatException::what() const throw()
{
	return ("Error parsing the file");
};
