#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "Server.hpp"

#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;
/**
 * @class
 * Config
 * 
 * @brief
 * This class encompasses all of the configurations that are available
 * for the webserv program. Using this class you can read and set the
 * configurations
 */

enum E_Types {
	E_port = 0,
	E_servername,
	E_errorpage,
	E_client_max_body,
	E_root,
	E_index,
	E_location,
	E_name,
	E_methods,
	E_autoindex,
	E_upload,
	E_loc_handling
};

class Config
{
	public:
		Config(void);
		Config(const char* fileLocation);
		Config(const Config& other);
        Config(int port);
		~Config(void);

		Config &operator=(const Config& other);

		// ------------------- Config getters ------------------------

        int getPort(void) const { return this->port; };
        // ------------------- Config member functions ------------------------

		void setWithFile(const char *fileLocation);
		
		void	openConfig(ifstream& config_file, int argc, char *argv[]);
		void	initialiseConfig(int argc, char *argv[]);
		void	createConfig(ifstream& config_file);
		// ------------------ ConfigException functions -----------------------

		class FileNotRetrievedException : public std::exception
		{
		public:
			virtual const char *what() const throw();
		};

		class FileIncorrectFormatException : public std::exception
		{
		public:
			virtual const char *what() const throw();
		};


	private:
		map<std::string, map<std::string, int> > _mapConfig;
		// File
		std::ifstream		_configFile;
		std::string			_fileLocation;

		std::vector<Server> _servers;

		// Misc configs
		int		        port;
		std::string		server_name;
		std::string		error_page; // can be multiple so best make map
		std::string		client_max_body_size; // can be formatted in different ways
		std::string		root;
		std::string		index;
		std::vector<std::string*> location;
		// Location		*location; // Can be multiple make vector or map
			// root
			// name
			// index
			// allow_methods
			// autoindex
			// upload store
			// location handling
		// uint32_t		port;
		// std::string		error_page; // can be multiple so best make map


};

#endif