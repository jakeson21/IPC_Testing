cmake_minimum_required(VERSION 3.1)
project(ipc_testing)

set(PROJECT_VERSION 1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message("Looking for Boost")
find_package(Boost REQUIRED)

link_directories(${CMAKE_INSTALL_PREFIX}/bin/lib)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/bin/lib")

add_subdirectory(ipc)


add_executable(ipc_client client.cpp)
target_link_libraries(ipc_client PUBLIC myIpcLib pthread)
target_include_directories(ipc_client PRIVATE ipc)


add_executable(ipc_server server.cpp)
target_link_libraries(ipc_server PUBLIC myIpcLib pthread)
target_include_directories(ipc_server PRIVATE ipc)


install(TARGETS ipc_client RUNTIME DESTINATION bin)
install(TARGETS ipc_server RUNTIME DESTINATION bin)



