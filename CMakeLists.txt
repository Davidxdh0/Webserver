cmake_minimum_required(VERSION 3.25)
project(Webserv)

set(CMAKE_CXX_STANDARD 98)

include_directories(include)

add_executable(Webserv_dev
        include/Config.h
        include/Server.h
        include/ServerControl.h
        include/utils.h
        include/Client.h
        include/Request.h
        include/Response.h
        include/Path.h
        src/Config.cpp
        src/main.cpp
        src/Server.cpp
        src/ServerControl.cpp
        src/Client.cpp
        src/Request.cpp
        src/Response.cpp
        src/utils.cpp
        src/Path.cpp src/Settings.cpp src/FileHandling.cpp src/ListDirectories.cpp src/ParseConfig.cpp)
