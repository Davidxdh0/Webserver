/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: rubennijhuis <rubennijhuis@student.coda      +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/13 16:38:00 by rubennijhui   #+#    #+#                 */
/*   Updated: 2023/09/13 15:35:03 by dyeboa        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Config.h"
#include "ServerControl.h"
#include "ParseConfig.h"
#include "utils.h"
#include <unistd.h>
#include <fstream>

using namespace std;

Config* dummy_configs()
{
    Config*  port_configs = new Config[3];
    Settings* settings = new Settings[3];

    port_configs[0].setPort(8080);
    port_configs[1].setPort(8081);
    port_configs[2].setPort(0);

    settings[0].setHost("localhost:8080");
    settings[0].setRoot("/Users/ajanse/Webserv_dev/public");
    settings[0].setIndex("index.php");
    settings[0].setAllowMethods(3);
    settings[0].setAutoindex("true");
    settings[0].setCgiPath("/cgi-bin");
    settings[0].setCgiExtension(".php");
    settings[0].setUploadPath("/uploads");
    settings[0].setClientMaxBodySize(1000000);

    settings[1].setHost("localhost");
    settings[1].setRoot("/public");
    settings[1].setIndex("index1.html");
    settings[1].setAllowMethods(3);
    settings[1].setAutoindex("true");
    settings[1].setCgiPath("/cgi-bin");
    settings[1].setCgiExtension(".php");
    settings[1].setUploadPath("/uploads");
    settings[1].setClientMaxBodySize(1000000);

    settings[2].setHost("");

    port_configs[0].setHosts(settings);
    return port_configs;
}

int main(int argc, char* argv[])
{
    Config*  port_configs;
	
    port_configs = dummy_configs();

	if (argc == 2){
		ParseConfig config(argv[1]);
		std::vector<pair<int, Settings* > > Config_Vect = config.ParseConfigFile();
		// config.PrintVector(Config_Vect);
	}

    // ServerControl   serverControl(port_configs);
    return 0;
}
