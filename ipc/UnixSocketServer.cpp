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

void UnixSocketServer::start()
{
    this->initSocket();
    unlink(server.sun_path);
    if (bind(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un))) 
    {
        perror("binding stream socket");
        throw std::runtime_error("Error binding stream socket");
    }
    //printf("Socket has name %s\n", server.sun_path);
    listen(sock, 5);
    isConnected = true;
}


} /* namespace ipc */


