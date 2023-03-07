cmake_minimum_required(VERSION 3.24)
project(SimpleBackEndProgram)

set(CMAKE_CXX_STANDARD 11)
add_executable(client client/client.hpp client/client.cpp client/main.cpp)
add_executable(SimpleBackEndProgram main.cpp server/server.hpp server/server.cpp )

