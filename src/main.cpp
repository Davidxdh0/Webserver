/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: rubennijhuis <rubennijhuis@student.coda      +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/13 16:38:00 by rubennijhui   #+#    #+#                 */
/*   Updated: 2023/09/28 12:08:41 by dyeboa        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Config.h"
#include "ServerControl.h"
#include "ParseConfig.h"
#include "utils.h"
#include <unistd.h>
#include <fstream>

using namespace std;

void	Settings::freeSettings(std::vector<std::pair<int, Settings* > > &Config_Vector){
	std::vector<std::pair<int, Settings* > >::iterator it;
	std::vector<std::pair<std::string, Settings* > >::iterator it_location;
	for (it = Config_Vector.begin(); it != Config_Vector.end(); it++){
		std::vector<std::pair<std::string, Settings*> > locationsCopy = Config_Vector.begin()->second->getLocations();
    	for (it_location = locationsCopy.begin(); it_location != locationsCopy.end(); it_location++) {
			if (it_location->second != NULL)
				delete it_location->second;
    	}
		delete it->second;
		
	}
}

int main(int argc, char* argv[])
{
    std::vector<pair<int, Settings* > > Config_Vect;
	if (argc == 2){
		ParseConfig config(argv[1]);
		Config_Vect = config.ParseConfigFile();
       	// config.PrintVector(Config_Vect);
		ServerControl   serverControl(Config_Vect);
		Settings::freeSettings(Config_Vect);
	}
    return 0;
}
