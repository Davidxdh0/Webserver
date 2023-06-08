#ifndef SERVER_HPP
# define SERVER_HPP

class Server
{
	public:
		Server(void);
		Server(const Server& other);
		~Server(void);

		Server &operator=(const Server& other);
};

#endif