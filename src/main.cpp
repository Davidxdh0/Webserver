/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: rubennijhuis <rubennijhuis@student.coda      +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/13 16:38:00 by rubennijhui   #+#    #+#                 */
/*   Updated: 2023/06/06 13:22:53 by rubennijhui   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Config.h"
#include "ServerControl.h"
#include <vector>
#include "utils.h"

using namespace std;

int main()
{
    vector<Config>  configs;
    Config          config1(8080);
    Config          config2(8081);

    configs.push_back(config1);
    configs.push_back(config2);

    log("Starting server control" + to_string(configs[1].getPort()));
    ServerControl   serverControl(configs);
    return 0;
}
