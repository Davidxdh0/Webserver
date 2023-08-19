/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: rubennijhuis <rubennijhuis@student.coda      +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/13 16:38:00 by rubennijhui   #+#    #+#                 */
/*   Updated: 2023/08/19 18:37:31 by dyeboa        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Config.h"
#include "ServerControl.h"
#include <vector>

using namespace std;

int main()
{
    vector<Config>  configs;
    Config          config1(8080);
    Config          config2(8081);

    configs.push_back(config1);
    configs.push_back(config2);

    ServerControl   serverControl(configs);
    return 0;
}
