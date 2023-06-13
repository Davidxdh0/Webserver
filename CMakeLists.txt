cmake_minimum_required(VERSION 3.25)
project(Webserv)

set(CMAKE_CXX_STANDARD 98)

include_directories(include)

add_executable(Webserv_dev
        include/Config.h
        include/ServerBlock.h
        include/ServerControl.h
        include/utils.h
        src/Config.cpp
        src/main.cpp
        src/ServerBlock.cpp
        src/ServerControl.cpp
        src/utils.cpp)
