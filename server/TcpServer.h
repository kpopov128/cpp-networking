#pragma once

#include "Socket.h"
#include "NetworkRuntime.h"

#include <cstdint>
#include <string>

namespace networking
{

class TcpServer
{
public:
    TcpServer(const std::string& address, uint16_t port);

    void Run();

private:
    void HandleClient(Socket clientSocket);

private:
    NetworkRuntime winsockRuntime;
    Socket serverSocket;
};

}
