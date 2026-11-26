#pragma once

#include "Socket.h"

#include <string>
#include <cstdint>
#include <optional>

namespace networking
{

class TcpClient
{
public:
    TcpClient(const std::string& address, uint16_t port);

    void Connect();

    void SendMessage(const std::string& message);
    std::optional<std::string> ReceiveMessage();

private:
    Socket clientSocket;
    std::string serverAddress;
    uint16_t serverPort;
};

}
