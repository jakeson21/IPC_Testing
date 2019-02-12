// UNIX Domain Sockets
#ifndef UNIXSOCKET_H
#define UNIXSOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <chrono>
#include <thread>
#include <mutex>
#include <stdexcept>
#include <string>
#include <iostream>
#include <map>
#include <queue>

namespace ipc {
    
    struct __attribute__ ((packed)) Data{
        Data()
        : length(4*102400)
        {
        }
            
        size_t length;
        char buf[4*102400];
    };
    
    enum class StatusTypeE {
        Success = 0,
        Connection_Error = 1,
        BindError,
        SetSockOptError,
        SocketOpenError
    };
    
    
    class UnixSocket
    {
    public:
        UnixSocket()
        : sock(0),
        myName("socket"),
        isConnected(false),
        isRunning(true),
        poolJoinThread(&UnixSocket::cleanUp, this)
        {
        }
        
        UnixSocket(const std::string& inName)
        : myName(inName),
        isConnected(false),
        isRunning(true),
        poolJoinThread(&UnixSocket::cleanUp, this)
        {
        }
        
        
        virtual ~UnixSocket()
        {
            stop();
        }
        
        virtual StatusTypeE start() = 0;
        
        void stop() 
        { 
            isConnected = false;
            isRunning = false;
            if (sock) close(sock);
            sock = 0;
            
            if (poolJoinThread.joinable())
            {
                poolJoinThread.join();
            }
            
            for (auto& item : threadMap)
            {
                auto t = item.second.get();
                if (t->joinable())
                {
                    t->join();
                }
            }
            threadMap.clear();
        }
        
        bool send(const ipc::Data& inData)
        {
            if (write(sock, inData.buf, inData.length) < 0)
            {
                perror("writing on stream socket");
                return false;
            }
            return true;
        }
        
        void receive()
        {
            if (isConnected)
            {
                // int rval=0;
                socklen_t clilen;
                struct sockaddr cli_addr;
                clilen = sizeof(cli_addr);
//                 char s[INET6_ADDRSTRLEN];
                
                int msgsock = accept(sock, (struct sockaddr *) &cli_addr, &clilen);
                if (msgsock < 0)
                {
                    perror("accept\n");
                    exit(1);
                }
                
    //             inet_ntop(cli_addr.ss_family, 
    //                       get_in_addr((struct sockaddr *)&cli_addr), 
    //                       s, 
    //                       sizeof(s));
    //             
    //             printf("server: got connection from %s\n", s);
                
//                 printf("server: got connection from %s\n", cli_addr.sa_data);
                
                // printf("server: got connection\n");
                
                static size_t id = 0;
                {
                    std::lock_guard<std::mutex> lock(threadMapMutex);
                    threadMap[id] = std::make_shared<std::thread>(&UnixSocket::rxThread, this, msgsock, id);
                    std::cout << "Added > thread pool size: " << threadMap.size() << std::endl;
                }
                id++;
                
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
        
        void rxThread(int handle, size_t threadId)
        {
            // std::thread::id this_id = std::this_thread::get_id();
            // std::cout << "thread's id: " << this_id << std::endl;
            
            ipc::Data d;
            bzero(d.buf, d.length*sizeof(char));
            int rval = read(handle, d.buf, d.length);
            close(handle);
            // printf("read %d-->%s", rval, d.buf);
            std::cout << "Got " << rval << " bytes" << std::endl;
            
            // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            
            // Prep for thread join()
            std::lock_guard<std::mutex> lock(threadJoinQueueMutex);
            threadJoinQueue.push(threadId);
        }
        
    protected:
        StatusTypeE initSocket()
        {
            sock = socket(AF_UNIX, SOCK_STREAM, 0);
            if (sock < 0) {
                perror("opening stream socket");
                // throw std::runtime_error("Error opening stream socket");
                return StatusTypeE::SocketOpenError;
            }
            server.sun_family = AF_UNIX;
            strcpy(server.sun_path, myName.c_str());            
            return StatusTypeE::Success;
        }
        
        // get sockaddr, IPv4 or IPv6:
        void *get_in_addr(struct sockaddr *sa)
        {
            if (sa->sa_family == AF_INET) {
                return &(((struct sockaddr_in*)sa)->sin_addr);
            }
            return &(((struct sockaddr_in6*)sa)->sin6_addr);
        }
        
        void cleanUp()
        {
            std::cout << "cleanUp() thread" << std::endl;
            while (isRunning)
            {                                
                std::unique_lock<std::mutex> lock(threadJoinQueueMutex);
                if (threadJoinQueue.size())
                {
                    size_t threadMapKey = threadJoinQueue.front();
                    threadJoinQueue.pop();
                    lock.unlock();
                    
                    std::lock_guard<std::mutex> lock2(threadMapMutex);
                    auto search = threadMap.find(threadMapKey);
                    if (search != threadMap.end())
                    {
                        auto t = search->second.get();
                        if (t->joinable())
                        {
                            t->join();
                        }
                        threadMap.erase(threadMapKey);
                        std::cout << "Erased > thread pool size: " << threadMap.size() << std::endl;
                    }
                }
                else
                {
                    lock.unlock();
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
            }
        }
        
        int sock;
        // int msgsock;
        struct sockaddr_un server;
        std::string myName; // must be < 108 characters in length
        
        bool isConnected, isRunning;
        
        std::mutex threadMapMutex;
        std::map<size_t, std::shared_ptr<std::thread>> threadMap;
        
        std::thread poolJoinThread;
        std::mutex threadJoinQueueMutex;
        std::queue<size_t> threadJoinQueue;
    };
        
}

#endif // UNIXSOCKET_H
