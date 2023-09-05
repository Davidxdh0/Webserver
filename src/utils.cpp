//
// Created by Alle Janse on 6/6/23.
//

#include <iostream>

void log(const std::string &message)
{
    std::cout << message << std::endl;
}

void exitWithError(const std::string &errorMessage)
{
    log("ERROR: " + errorMessage);
    exit(1);
}