//
// Created by Alle Janse on 8/29/23.
//

#include "Settings.h"

Settings::Settings() : _allow_methods(0), _autoindex(), _client_max_body_size(1000000)
{
}

Settings::Settings(const Settings &src)
{
    *this = src;
}

Settings::~Settings()
{
}


