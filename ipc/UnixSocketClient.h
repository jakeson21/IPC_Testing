// UNIX Domain Sockets
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
        virtual ~UnixSocketClient();
        
        void start();
        
    protected:
//         std::vector<char> buf;
        
    };

} /* namespace ipc */

