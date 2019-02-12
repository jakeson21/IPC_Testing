#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <cstdio>
#include <string>

#include <chrono>
#include <ctime>
#include <thread>
#include <memory>
#include <iostream>

#include <UnixSocketClient.h>

#include <sstream>

volatile sig_atomic_t flag = 0;
void my_handler(int){
//     printf("Caught signal %d\n",s);
    flag = 1;
}


void f1(ipc::UnixSocketClient& client)
{
    ipc::Data data;
    std::chrono::system_clock::time_point today;
    while(!flag)
    {
        ipc::StatusTypeE status;
        if ((status = client.start()) != ipc::StatusTypeE::Success)
        {
            std::cout << "f1 Error " << (int)status << std::endl;
            return;
        }
        
        today = std::chrono::system_clock::now();
        time_t tt = std::chrono::system_clock::to_time_t ( today );
        char* d =  ctime(&tt);
        strncpy(data.buf, d, data.length);
        // data.length = strlen(d);
        
        printf("Sending: %s", data.buf);
        client.send(data);                        
        client.stop();
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void f2(ipc::UnixSocketClient& client)
{
    ipc::Data data;
    static int d = 0;
    while(!flag)
    {
        ipc::StatusTypeE status;
        if ((status = client.start()) != ipc::StatusTypeE::Success)
        {
            std::cout << "f2 Error " << (int)status << std::endl;
            return;
        }
        snprintf (data.buf, data.length, "Loop %d\n", ++d);
        printf("Sending: %s", data.buf);
        client.send(data);
        client.stop();
        // std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }    
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
    
    ipc::UnixSocketClient client1(std::string("myunixsocket"));
    ipc::UnixSocketClient client2(std::string("myunixsocket"));
    ipc::UnixSocketClient client3(std::string("myunixsocket"));
    ipc::UnixSocketClient client4(std::string("myunixsocket"));
    
    std::thread t1(f1, std::ref(client1));
    std::thread t2(f2, std::ref(client2));
    std::thread t3(f2, std::ref(client3));
    std::thread t4(f2, std::ref(client4));
    
    while(!flag)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    std::cout << "Client Exiting..." << std::endl;
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    
    return 0;
}
