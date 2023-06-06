//
// Created by Alle Janse on 6/6/23.
//

#include <iostream>

using namespace std;

void log(const string &message)
{
    cout << message << endl;
}

void exitWithError(const string &errorMessage)
{
    log("ERROR: " + errorMessage);
    exit(1);
}