//
// Created by Alle Janse on 6/6/23.
//

#ifndef WEBSERV_DEV_SERVERCONTROL_H
#define WEBSERV_DEV_SERVERCONTROL_H

#include "ServerBlock.h"
#include "Config.h"
#include <vector>
#include <sys/event.h>

using namespace std;

class ServerControl
{
public:
    ServerControl();
    explicit ServerControl(vector<Config> configs);
    ~ServerControl();

private:
    int             _kq_fd;
    vector<ServerBlock>  _servers;
    struct kevent   _events;

    ServerBlock*            checkIdentIsServer(int ident);
};
#endif //WEBSERV_DEV_SERVERCONTROL_H
