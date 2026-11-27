#pragma once

#include "Socket.h"
#include "NetworkRuntime.h"
#include "ClientConnection.h"
#include "TcpServerEvent.h"

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>


namespace networking
{

class TcpServer
{
public:
    TcpServer(const std::string& address, uint16_t port);

    void Run();
    void Stop();

private:
    void AcceptClients();

    TcpServerEvent WaitForEvent();

private:
    std::mutex eventsMutex;
    std::condition_variable eventsCondition;
    NetworkRuntime winsockRuntime;
    Socket serverSocket;
    std::queue<TcpServerEvent> serverEvents;
    std::atomic<bool> shutdownRequested{false};
    std::unordered_map<std::thread::id, std::unique_ptr<ClientConnection>> clientConnections;
};

}
