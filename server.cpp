#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>

#include <chrono>
#include <thread>

#include <UnixSocketServer.h>

volatile sig_atomic_t flag = 0;
void my_handler(int){
//     printf("Caught signal %d\n",s);
    flag = 1;
}

int main(int argc, char **argv) {
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = my_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);
    sigaction(SIGTERM, &sigIntHandler, NULL);
    sigaction(SIGKILL, &sigIntHandler, NULL);
   
    std::cout << "Starting Server..." << std::endl;
    
    ipc::UnixSocketServer server(std::string("myunixsocket"));
    ipc::StatusTypeE status;
    if ((status = server.start()) != ipc::StatusTypeE::Success)
    {
        return (int)status;
    }
    
    while(!flag)
    {
        // std::this_thread::sleep_for(std::chrono::milliseconds(1));
        // std::cout << "calling receive" << std::endl;
        server.receive();
    }
    
    std::cout << "Server Exiting..." << std::endl;
    
    return 0;
}
