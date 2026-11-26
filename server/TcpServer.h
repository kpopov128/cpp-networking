#pragma once

#include "Socket.h"
#include "NetworkRuntime.h"
#include "ClientConnection.h"

#include <cstdint>
#include <string>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <thread>

#include <queue>

namespace networking
{

class TcpServer
{
public:
    TcpServer(const std::string& address, uint16_t port);

    void Run();

private:
    void AcceptClients();

private:
    std::mutex acceptedClientsMutex;
    NetworkRuntime winsockRuntime;
    Socket serverSocket;
    std::queue<Socket> acceptedClients;
    std::unordered_map<std::thread::id, std::unique_ptr<ClientConnection>> clientConnections;
};

}
