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

void UnixSocketClient::start()
{
    this->initSocket();
    if (connect(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un)) < 0) {
        this->stop();
        perror("connecting stream socket");
        throw std::runtime_error("Error connecting stream socket");
    }
    //printf("Socket has name %s\n", server.sun_path);
    isConnected = true;
}

} /* namespace ipc */


