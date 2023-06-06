//
// Created by Alle Janse on 6/6/23.
//

#include <sys/event.h>
#include <unistd.h>
#include "../include/ServerControl.h"
#include "../include/utils.h"

ServerControl::ServerControl() : kqueue_fd(kqueue()), servers()
{
    if (kqueue_fd == -1)
        exitWithError("Failed to create kqueue");
}

ServerControl::ServerControl(vector<Config> configs) : kqueue_fd(kqueue()), servers()
{
    size_t size = configs.size();
    log(to_string(size));
    if (kqueue_fd == -1)
        exitWithError("Failed to create kqueue");
    for (size_t i = 0; i < size; i++)
    {
        log("Starting server on port: " + to_string(i));
        Server  tmp("0.0.0.0", configs[i].getPort());
        tmp.startListen();
        servers.push_back(tmp);
    }
}

ServerControl::~ServerControl()
{
    close(kqueue_fd);
}
