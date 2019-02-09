// UNIX Domain Sockets
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
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
        : sock(0),
        msgsock(0),
        myName("socket"),
        isConnected(false)
        {
            //initSocket();
        }
        
        UnixSocket(const std::string& inName)
        : myName(inName),
        isConnected(false)
        {
            //initSocket();
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
            sock = 0;
            msgsock = 0;
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
            int pid, rval=0;
            socklen_t clilen;
            struct sockaddr_in serv_addr, cli_addr;
            clilen = sizeof(cli_addr);
            ipc::Data d;
            
            //printf("accepting\n");
            msgsock = accept(sock, (struct sockaddr *) &cli_addr, &clilen);
            if (msgsock < 0)
                perror("accept\n");          

            //printf("accepted\n");
            pid = fork();

            bzero(d.buf, d.length*sizeof(char));
            if (pid < 0)
                perror("ERROR on fork");
            if (pid == 0)  {
                close(sock);
                rval = read(msgsock, d.buf, d.length);
                printf("-->%s", d.buf);
                exit(0);
            }
            else close(msgsock);
            
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
        struct sockaddr_un server;
        std::string myName; // must be < 108 characters in length
        
        bool isConnected;
    };
        
}
