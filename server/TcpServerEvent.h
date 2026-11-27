#pragma once

#include "Socket.h"

#include <optional>
#include <thread>
#include <utility>

namespace networking
{

struct TcpServerEvent
{
public:
    enum class Type
    {
        eClientAccepted = 0,
        eClientDisconnected,
        eShutdownRequested,
    };

    static TcpServerEvent CreateClientAccepted(Socket&& socket)         { return TcpServerEvent(std::move(socket)); }
    static TcpServerEvent CreateClientDisconnected(std::thread::id id)  { return TcpServerEvent(id); };
    static TcpServerEvent CreateShutdownRequested()                     { return TcpServerEvent(TcpServerEvent::Type::eShutdownRequested); }

private:
    explicit TcpServerEvent(const TcpServerEvent::Type eventType)
        : eventType(eventType)
    {
    }

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

}
