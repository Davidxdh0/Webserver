//
// Created by Alle Janse on 6/6/23.
//

#ifndef WEBSERV_DEV_UTILS_H
#define WEBSERV_DEV_UTILS_H

#include <iostream>
#include "Settings.h"

typedef enum e_clientState {
    READING,
    RESPONDING,
    DISCONNECT
} 			clientState;

typedef enum e_requestMethod {
    GET,
    POST,
    DELETE
} 			requestMethod;


#define ROOT "/Users/dyeboa/Documents/Webserv/public";
#define INDEX "index.html";

void log(const std::string &message);
void exitWithError(const std::string &errorMessage);

#endif //WEBSERV_DEV_UTILS_H
