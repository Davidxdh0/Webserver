#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <fstream>
#include <string>

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
        Config(int port);
		~Config(void);

		Config &operator=(const Config& other);

		// ------------------- Config getters ------------------------

        int getPort(void) const { return this->port; };
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
		int		        port;
		std::string		server_name;
		std::string		error_page; // can be multiple so best make map
		std::string		client_max_body_size; // can be formatted in different ways

		// Location		*location; // Can be multiple make vector or map
			// name
			// root
			// index
			// allow_methods
			// autoindex
			// upload store
			// location handling

};

#endif