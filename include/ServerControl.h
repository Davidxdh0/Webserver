//
// Created by Alle Janse on 6/6/23.
//

#ifndef WEBSERV_DEV_SERVERCONTROL_H
#define WEBSERV_DEV_SERVERCONTROL_H

#include "Server.h"
#include "Config.h"
#include <vector>

using namespace std;

class ServerControl
{
public:
    ServerControl();
    explicit ServerControl(vector<Config> configs);
    ~ServerControl();

private:
    int             kqueue_fd;
    vector<Server>  servers;

};
#endif //WEBSERV_DEV_SERVERCONTROL_H
