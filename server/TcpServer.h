#pragma once

#include "Socket.h"
#include "WinsockRuntime.h"

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
    WinsockRuntime winsockRuntime;
    Socket serverSocket;
};

}
