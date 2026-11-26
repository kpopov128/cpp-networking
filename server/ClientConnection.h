#pragma once

#include "Socket.h"
#include "Thread.h"


namespace networking
{

class ClientConnection
{
public:
    explicit ClientConnection(Socket&& socket);

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
    Thread clientThread;
};

} // namespace networking
