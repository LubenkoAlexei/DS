cmake_minimum_required(VERSION 3.21)
project(Client)

set(CMAKE_CXX_STANDARD 98)
include_directories(../Server)
add_executable(Client main.cpp ../Server/employee.h)
