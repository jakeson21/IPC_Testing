#ifndef UNIXSOCKETSERVER_H
#define UNIXSOCKETSERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <vector>

#include <UnixSocket.h>
#define NAME "socket"

namespace ipc {
    
    class UnixSocketServer : public UnixSocket
    {
    public:
        UnixSocketServer();
        UnixSocketServer(const std::string& inName);
        virtual ~UnixSocketServer();
        
        StatusTypeE start();

    protected:
        
    };    

} /* namespace ipc */

#endif // UNIXSOCKETSERVER_H
