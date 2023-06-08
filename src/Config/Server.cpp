#include "Config/Server.hpp"
#include <iostream>

Server::Server(void)
{
    std::cout << "Server created with an empty construcor" << std::endl;

    return;
}

Server::~Server(void)
{
    std::cout << "Server deconstructed" << std::endl;
    return;
}

Server::Server(const Server& other)
{
    std::cout << "Server created by copy" << std::endl;
    *this = other;
    return;
}

Server& Server::operator=(const Server& other)
{
    std::cout << "Server created by assertion" << std::endl;

    if (this != &other)
    {

    }

    return (*this);
}
