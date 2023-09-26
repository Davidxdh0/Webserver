/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: rubennijhuis <rubennijhuis@student.coda      +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/13 16:38:00 by rubennijhui   #+#    #+#                 */
/*   Updated: 2023/09/26 21:57:18 by dyeboa        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Config.h"
#include "ServerControl.h"
#include "ParseConfig.h"
#include "utils.h"
#include <unistd.h>
#include <fstream>

using namespace std;

void leaks(void){
    system("leaks -q webserv_dev");
}

int main(int argc, char* argv[])
{
    std::vector<pair<int, Settings* > > Config_Vect;
   	// atexit(leaks);
	if (argc == 2){
		ParseConfig config(argv[1]);
		Config_Vect = config.ParseConfigFile();
       	// config.PrintVector(Config_Vect);
		ServerControl   serverControl(Config_Vect);
	}
    return 0;
}
