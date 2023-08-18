/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: rubennijhuis <rubennijhuis@student.coda      +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/13 16:38:00 by rubennijhui   #+#    #+#                 */
/*   Updated: 2023/08/18 14:25:35 by dyeboa        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Config/Config.hpp"

#include <sys/socket.h> 	// socket
#include <sys/types.h> 		// setsockopt
#include <netinet/in.h> 	// struct sockaddr_in
#include <string.h>  		// tostring c_str
#include <sstream>
int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Error: incorrect amount of arguments supplied!" << argv[0] << std::endl;
		// return (1);
	}

	Config config;

	try
	{
		int socketfd = socket(AF_INET, SOCK_STREAM, 0);
		if (socketfd == -1){
			throw std::runtime_error("Error: Socket -1");
			return 1;
		}
		int keepalive = 1;
		if (setsockopt(socketfd, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive)) == -1){
			perror("socket");
			throw std::runtime_error("Error: setsockopt keepalive -1");
			return 1;
		}

		struct sockaddr_in serverAddress = {
			.sin_family 	 = AF_INET, // 
			.sin_addr.s_addr = htonl(INADDR_LOOPBACK),
			.sin_port 		 = htons(8080)
		};

		if (bind(socketfd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
			perror("bind");
            throw std::runtime_error("Error: bind -1");
        }
		
		// config.setWithFile(argv[1]);
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}

	return (0);
}
