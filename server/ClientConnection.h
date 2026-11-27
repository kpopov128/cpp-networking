#pragma once

#include "Socket.h"
#include "Thread.h"

#include <functional>

namespace networking
{

class ClientConnection
{
public:
    ClientConnection(Socket&& socket, std::function<void(const std::thread::id)>&& disconnectedCb);

    ~ClientConnection() = default;

    ClientConnection(const ClientConnection&) = delete;
    ClientConnection(ClientConnection&&) = delete;

    ClientConnection& operator=(const ClientConnection&) = delete;
    ClientConnection& operator=(ClientConnection&&) = delete;

    std::thread::id GetThreadId() const noexcept { return clientThread.GetId(); }

private:
    void HandleConnection();

private:
    Socket clientSocket;
    std::function<void(const std::thread::id)> clientDisconnectedCb;
    Thread clientThread;
};

} // namespace networking
