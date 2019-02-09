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
    server.start();
    
//     while(!flag) {
//         msgsock = accept(sock, 0, 0);
//         if (msgsock == -1)
//             perror("accept");
//         else do {
//             bzero(buf.data(), buf.size()*sizeof(char));
//             if ((rval = read(msgsock, buf.data(), buf.size())) < 0)
//                 perror("reading stream message");
//             else if (rval == 0)
//                 printf("Ending connection\n");
//             else
//                 printf("-->%s\n", buf.data());
//         } while (rval > 0);
//         close(msgsock);
//     }
    
    
    ipc::Data d;
    while(!flag)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        
//         if (server.receive(d) > 0)
//         {
//             printf("-->%s", d.buf);
//         }
//         else
//         {
//             printf("nothing...\n");
//         }
        server.receive(d);
    }
    
    std::cout << "Server Exiting..." << std::endl;
    
    return 0;
}
