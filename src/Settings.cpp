//
// Created by Alle Janse on 8/29/23.
//

#include "Settings.h"

Settings::Settings() : _allow_methods(), _autoindex(), _client_max_body_size()
{
}

Settings::Settings(const Settings &src)
{
    *this = src;
}

Settings::~Settings()
{
}


