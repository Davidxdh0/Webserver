#include "../include/Config.h"
#include <iostream>

Config::Config(void)
{
	std::cout << "Config created with an empty construcor" << std::endl;
}

Config::~Config(void)
{
	std::cout << "Config deconstructed" << std::endl;
}

Config::Config(const Config &other)
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
		throw Config::FileNotFoundException();
	}

	if (this->_configFile.fail())
	{
		throw Config::FileNotFoundException();
	}

	this->_fileLocation = fileLocation;
}

// ---------------------- BureaucratException Functions ------------------------

const char *Config::FileNotFoundException::except() const throw()
{
	return ("Error finding or opening the file");
}

const char *Config::FileIncorrectFormatException::except() const throw()
{
	return ("Error parsing the file");
};
