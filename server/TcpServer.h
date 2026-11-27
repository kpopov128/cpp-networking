#pragma once

#include "Socket.h"
#include "NetworkRuntime.h"
#include "ClientConnection.h"

#include <condition_variable>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <thread>
#include <optional>
#include <queue>


namespace networking
{

struct TcpServerEvent
{
    enum class Type
    {
        eClientAccepted = 0,
        eClientDisconnected
    };

    TcpServerEvent() = default;

    explicit TcpServerEvent(Socket&& socket)
        : eventType(Type::eClientAccepted), clientSocket(std::move(socket))
    {
    }

    explicit TcpServerEvent(std::thread::id id)
        : eventType(Type::eClientDisconnected), threadId(id)
    {
    }

public:
    Type eventType;
    std::optional<Socket> clientSocket;
    std::optional<std::thread::id> threadId;
};

class TcpServer
{
public:
    TcpServer(const std::string& address, uint16_t port);

    void Run();

private:
    void AcceptClients();

private:
    std::mutex eventsMutex;
    std::condition_variable eventsCondition;
    NetworkRuntime winsockRuntime;
    Socket serverSocket;
    std::queue<TcpServerEvent> serverEvents;
    std::unordered_map<std::thread::id, std::unique_ptr<ClientConnection>> clientConnections;
};

}
