cmake_minimum_required(VERSION 3.5)
project(Project_3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    chat_client.c
    chat_server.c
    file_client.c
    file_server.c
    tcp.h)

add_executable(Project_3 ${SOURCE_FILES})