#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <chrono>
#include <ctime>
#include <thread>

#include <UnixSocketClient.h>

#include <sstream>

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
   
    std::cout << "Starting Client..." << std::endl;
    
    ipc::UnixSocketClient client;
    client.start();
    
    ipc::Data data;
    std::chrono::system_clock::time_point today;
    while(!flag)
    {
        today = std::chrono::system_clock::now();
        time_t tt = std::chrono::system_clock::to_time_t ( today );
        char* d =  ctime(&tt);
        strncpy(data.buf, d, data.length);
        // data.length = strlen(d);
        printf("Sending: %s", data.buf);
        client.send(data);

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    
    std::cout << "Client Exiting..." << std::endl;
    
    return 0;
}
