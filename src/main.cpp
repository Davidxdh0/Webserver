/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: rubennijhuis <rubennijhuis@student.coda      +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/13 16:38:00 by rubennijhui   #+#    #+#                 */
/*   Updated: 2023/09/04 21:00:39 by dyeboa        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Config.h"
#include "ServerControl.h"
#include <vector>
#include "utils.h"

using namespace std;

Config* dummy_configs()
{
    Config*  port_configs = new Config[3];
    Settings* settings = new Settings[2];

    port_configs[0].setPort(8080);
    port_configs[1].setPort(8081);
    port_configs[2].setPort(0);

    settings[0].setHost("localhost");
    settings[0].setRoot("/public");
    settings[0].setIndex("index.html");
    settings[0].setAllowMethods(3);
    settings[0].setAutoindex(true);
    settings[0].setCgiPath("/cgi-bin");
    settings[0].setCgiExtension(".php");
    settings[0].setUploadPath("/uploads");
    settings[0].setClientMaxBodySize(1000000);

    settings[1].setHost("localhost");
    settings[1].setRoot("/public");
    settings[1].setIndex("index.html");
    settings[1].setAllowMethods(3);
    settings[1].setAutoindex(true);
    settings[1].setCgiPath("/cgi-bin");
    settings[1].setCgiExtension(".php");
    settings[1].setUploadPath("/uploads");
    settings[1].setClientMaxBodySize(1000000);

    port_configs[0].setHosts(settings);
    return port_configs;
}

int main()
{
//    vector<Config>  configs;
//    Config          config1(8080);
//    Config          config2(8081);
//
//    configs.push_back(config1);
//    configs.push_back(config2);

    Config*  port_configs;

    port_configs = dummy_configs();
    ServerControl   serverControl(port_configs);
    return 0;
}
