#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <fstream>

/**
 * @class
 * Config
 * 
 * @brief
 * This class encompasses all of the configurations that are available
 * for the webserv program. Using this class you can read and set the
 * configurations
 */
class Config
{
	public:
		Config(void);
		Config(const Config& other);
		~Config(void);

		Config &operator=(const Config& other);

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
		// File
		std::ifstream	_configFile;
		std::string		_fileLocation;

		// Servers

		// Misc configs

};

#endif