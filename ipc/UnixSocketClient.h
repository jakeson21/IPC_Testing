#ifndef UNIXSOCKETCLIENT_H
#define UNIXSOCKETCLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <vector>

#include <UnixSocket.h>
#define NAME "socket"

namespace ipc {
    
    class UnixSocketClient : public UnixSocket
    {
    public:
        UnixSocketClient();
        UnixSocketClient(const std::string& inName);
        virtual ~UnixSocketClient();
        
        StatusTypeE start();
        
    protected:
        
    };

} /* namespace ipc */

#endif // UNIXSOCKETCLIENT_H
