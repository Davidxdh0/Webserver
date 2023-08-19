/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: rubennijhuis <rubennijhuis@student.coda      +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/13 16:38:00 by rubennijhui   #+#    #+#                 */
/*   Updated: 2023/08/19 13:57:24 by dyeboa        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Config/Config.hpp"

#include <sys/socket.h> 	// socket
#include <sys/types.h> 		// setsockopt
#include <netinet/in.h> 	// struct sockaddr_in
#include <string.h>  		// tostring c_str
#include <sstream>
#include <arpa/inet.h>		// inet_ntop
#include <unistd.h> 		// close 
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
		
		struct sockaddr_in serverAddress = {
			.sin_family 	 = AF_INET, // 
			.sin_addr.s_addr = htonl(INADDR_LOOPBACK),
			.sin_port 		 = htons(8000)
		};
		
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


		if (bind(socketfd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
			perror("bind");
            throw std::runtime_error("Error: bind -1");
        }
		if (listen(socketfd, 1) == -1){
			perror("socket");
            throw std::runtime_error("Error: socket -1");
		}
		// script -q /dev/null 
		// lsof -i :8000
		// config.setWithFile(argv[1]);
		while(true){
			struct sockaddr_in clientAddress;
			socklen_t addrlen = sizeof(struct sockaddr_in);
			int new_socket = accept(socketfd, (struct sockaddr *)&serverAddress, &addrlen);
			
			char addrStr[INET_ADDRSTRLEN];
			if (inet_ntop(AF_INET, &(clientAddress.sin_addr), addrStr, INET_ADDRSTRLEN) == NULL) {
				perror("inet_ntop");
				close(new_socket);
				throw std::runtime_error("Error: inet_ntop -1");
			}
			
			new_socket = accept(socketfd, (struct sockaddr *)&clientAddress, &addrlen);
			printf("Incoming connection from %s - sending welcome\n", addrStr);
			char buffer[1024] = { 0 };
			char hello[18] = "Hello from server";
			read(new_socket, buffer, 1024);
			printf("%s\n", buffer);
			
			send(new_socket, hello, strlen(hello), 0);
			printf("Hello message sent\n");
		
			// closing the connected socket
			close(new_socket);
			// closing the listening socket
			shutdown(socketfd, SHUT_RDWR);
		}
    
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}

	return (0);
}
