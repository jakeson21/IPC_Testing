set(PROJECT_VERSION 1)

# set_target_properties(myIpcLib PROPERTIES VERSION ${PROJECT_VERSION})

add_library(myIpcLib SHARED 
            UnixSocketClient.cpp
            UnixSocketServer.cpp
            )
            
target_include_directories(myIpcLib PRIVATE .)

install(TARGETS myIpcLib
    LIBRARY DESTINATION bin/lib)
    
