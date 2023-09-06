#include "../include/Config.h"
#include "Path.h"
#include <iostream>

Config::Config() : _port()
{
	std::cout << "Config created with an empty construcor" << std::endl;
}

Config::~Config()
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

//Settings Config::getRightSettings(const std::string& host,Path uri) {
//    Settings ret;
//    int i = 0;
//    while(!_hosts[i].getHost().empty()) {
//        if (_hosts[i].getHost() == host) {
//            ret = _hosts[i].getRightSettings(uri);
//            break;
//        }
//        i++;
//    }
//    return ret;
//}



// ---------------------- Config member functions ------------------------------

// void Config::setWithFile(const char *fileLocation)
// {
// 	if (!this->_configFile.is_open())
// 	{
// 		this->_configFile.close();
// 	}

// 	this->_configFile.open(fileLocation);

// 	if (!this->_configFile.is_open())
// 	{
// 		throw Config::FileNotFoundException();
// 	}

// 	if (this->_configFile.fail())
// 	{
// 		throw Config::FileNotFoundException();
// 	}

// 	this->_fileLocation = fileLocation;
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
