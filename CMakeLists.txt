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
        include/Settings.h
        include/MimeTypes.h
        include/ParseConfig.h

        src/CgiResponse.cpp
        src/Client.cpp
        src/Config.cpp
        src/main.cpp
        src/MimeTypes.cpp
        src/ParseConfig.cpp
        src/ParseConfigUtil.cpp
        src/ParseVariables.cpp
        src/Path.cpp
        src/Request.cpp
        src/Response.cpp
        src/ResponseDirectory.cpp
        src/ResponseUploadDelete.cpp
        src/Server.cpp
        src/ServerControl.cpp
        src/Settings.cpp
        src/utils.cpp
        )
