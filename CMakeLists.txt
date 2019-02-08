cmake_minimum_required(VERSION 2.6)
project(ipc_testing)

add_executable(ipc_testing main.cpp)

install(TARGETS ipc_testing RUNTIME DESTINATION bin)
