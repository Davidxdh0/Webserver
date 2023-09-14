//
// Created by Alle Janse on 6/6/23.
//

#ifndef WEBSERV_DEV_SERVERCONTROL_H
#define WEBSERV_DEV_SERVERCONTROL_H

#include "Server.h"
#include "Config.h"
#include <vector>
#include <sys/event.h>

using namespace std;

class ServerControl
{
public:
    ServerControl();
    explicit ServerControl(std::vector<pair<int, Settings* > > port_configs);
    ~ServerControl();

private:
    int             _kq_fd;
    vector<Server>  _servers;

    Server*     checkIdentIsServer(int ident);
    void        loadServers(std::vector<pair<int, Settings* > > port_configs);
    void        webservLoop();
};
#endif //WEBSERV_DEV_SERVERCONTROL_H
