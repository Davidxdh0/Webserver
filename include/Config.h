#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <fstream>
#include <string>
#include "Settings.h"

/**
 * @class
 * Config
 * 
 * @brief
 * This class encompasses all of the configurations that are available
 * for the webserv program. Using this class you can read and set the
 * configurations
 */
class Config {
	public:
		Config();
		Config(const Config& other);
        Config(int port);
		~Config();

		Config &operator=(const Config& other);

        // ------------------- Config setters ------------------------
        void    setPort(int port) { this->_port = port; };
        void    setHosts(Settings* hosts) { this->_hosts = hosts; };
		// ------------------- Config getters ------------------------

        int getPort() const { return this->_port; };
        Settings* getHosts() const { return this->_hosts; };
        // ------------------- Config member functions ------------------------

		void setWithFile(const char *fileLocation);

		// ------------------ ConfigException functions -----------------------

		class FileNotFoundException : public std::exception
		{
		public:
			virtual const char *except() const throw();
		};

		class FileIncorrectFormatException : public std::exception
		{
		public:
			virtual const char *except() const throw();
		};


	private:

		int		        _port;
        Settings*       _hosts;
};

#endif