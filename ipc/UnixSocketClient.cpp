// UNIX Domain Sockets
#include <UnixSocketClient.h>
#include <unistd.h>

#define DATA "socket"

namespace ipc {

UnixSocketClient::UnixSocketClient()
{
}

UnixSocketClient::UnixSocketClient(const std::string& inName)
: UnixSocket(inName)
{
}

UnixSocketClient::~UnixSocketClient()
{
}

StatusTypeE UnixSocketClient::start()
{
    ipc::StatusTypeE status;
    if ((status = this->initSocket()) != ipc::StatusTypeE::Success)
    {
        return status;
    }
    
    if (connect(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un)) < 0) {
        this->stop();
        perror("connecting stream socket");
        return StatusTypeE::Connection_Error;
    }

    isConnected = true;
    
    return StatusTypeE::Success;
}

} /* namespace ipc */


