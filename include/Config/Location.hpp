#ifndef LOCATION_HPP
# define LOCATION_HPP

#include <string>
#include <vector>

class Location
{
	public:
		Location(void);
		Location(std::string config);
		Location(const Location &other);
		~Location(void);

		Location &operator=(const Location& other);

		// ------------------- Config member functions ------------------------

		// ------------------ ConfigException functions -----------------------

		// class FileNotRetrievedException : public std::exception
		// {
		// public:
		// 	virtual const char *what() const throw();
		// };

	private:
		std::string					_selector;
		std::string					_root;
		std::string					_index;
		std::vector<std::string>	_methods;
		bool						_autoindex;
		std::string					_upload_store;
		std::vector<Location>		_locations;
};

#endif