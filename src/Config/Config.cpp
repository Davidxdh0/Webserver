#include "../include/Config.h"
#include "Config/Config.hpp"
#include <iostream>

Config::Config() : port()
{
	std::cout << "Config created with an empty construcor" << std::endl;
}

Config::~Config(){}

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

// ---------------------- Config Exception Functions ------------------------

const char *Config::FileNotRetrievedException::what() const throw()
{
	return ("Error finding or opening the file");
}

const char *Config::FileIncorrectFormatException::what() const throw()
{
	return ("Error parsing the file");
};
