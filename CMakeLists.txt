cmake_minimum_required(VERSION 4.2)
project(Fiscato_Lorenzo_Es_Named_Pipe C)

set(CMAKE_C_STANDARD 23)

add_executable(Es1-Client Es1/Client.c)
add_executable(Es1-Server Es1/Server.c)
