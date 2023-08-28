#include "Location.hpp"

#include <string>
#include <iostream>

Location::Location(void)
{
    std::cout << "Location created with an empty construcor" << std::endl;
    return;
}

Location::~Location(void)
{
    std::cout << "Location deconstructed" << std::endl;
    return;
}

Location::Location(std::string config)
{
    std::cout << "Location, " << config << ", created with a config string construcor" << std::endl;
    return;
}

Location::Location(const Location& other)
{
    std::cout << "Location created by copy" << std::endl;
    *this = other;
    return;
}

Location& Location::operator=(const Location& other)
{
    std::cout << "Location created by assertion" << std::endl;

    if (this != &other)
    {
        *this = other;
    }

    return (*this);
}
