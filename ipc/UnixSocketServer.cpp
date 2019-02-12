// UNIX Domain Sockets
#include <UnixSocketServer.h>

namespace ipc {

UnixSocketServer::UnixSocketServer()
{
}

UnixSocketServer::UnixSocketServer(const std::string& inName)
: UnixSocket(inName)
{
}

UnixSocketServer::~UnixSocketServer()
{
}

StatusTypeE UnixSocketServer::start()
{
    ipc::StatusTypeE status;
    if ((status = this->initSocket()) != ipc::StatusTypeE::Success)
    {
        return status;
    }

    unlink(server.sun_path);
    
    int yes=1;
    //char yes='1'; // Solaris people use this
    // lose the pesky "Address already in use" error message
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
        perror("setsockopt");
        // throw std::runtime_error("Error in setsockopt");
        return StatusTypeE::SetSockOptError;
    }
    
    if (bind(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un))) 
    {
        perror("binding stream socket");
        // throw std::runtime_error("Error binding stream socket");
        return StatusTypeE::BindError;
    }
    
    listen(sock, 5);
    isConnected = true;
    
    return StatusTypeE::Success;
}


} /* namespace ipc */


