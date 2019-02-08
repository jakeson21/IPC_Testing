// UNIX Domain Sockets
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <stdexcept>
#include <string>

namespace ipc {
    
    struct __attribute__ ((packed)) Data{
        Data()
        : length(128)
        {
            //buf.resize(1024, 0);
        }
            
        uint16_t length;
        char buf[128];
    };
    
    
    class UnixSocket
    {
    public:
        UnixSocket()
        : myName("socket"),
        isConnected(false)
        {
            initSocket();
        }
        
        UnixSocket(std::string inName)
        : myName(inName),
        isConnected(false)
        {
            initSocket();
        }
        
        
        virtual ~UnixSocket()
        {
            stop();
        }
        
        virtual void start() = 0;
        
        void stop() 
        { 
            if (sock) close(sock);
            if (msgsock) close(msgsock);
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
        
        int receive(ipc::Data& inBuf)
        {
            //msgsock = accept(sock, 0, 0);
            //if (msgsock == -1)
            //    perror("accept");
            //{
                bzero(inBuf.buf, inBuf.length*sizeof(char));
                //if ((rval = read(msgsock, inBuf.buf.data(), inBuf.buf.size())) < 0)
                //    perror("reading stream message");
                //else if (rval == 0)
                //    printf("Ending connection\n");
                //else
                //    printf("-->%s\n", buf.data());
            //}
            //close(msgsock);
            
            rval = read(msgsock, inBuf.buf, inBuf.length);
            return rval;
        }
        
    protected:
        void initSocket()
        {
            sock = socket(AF_UNIX, SOCK_STREAM, 0);
            if (sock < 0) {
                perror("opening stream socket");
                throw std::runtime_error("Error opening stream socket");
            }
            server.sun_family = AF_UNIX;
            strcpy(server.sun_path, myName.c_str());            
        }
        
        int sock;
        int msgsock;
        int rval;
        struct sockaddr_un server;
        std::string myName; // must be < 108 characters in length
        
        bool isConnected;
    };
        
}
