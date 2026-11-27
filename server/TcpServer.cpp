#include "TcpServer.h"
#include "Thread.h"

#include <iostream>
#include <utility>

namespace networking
{

TcpServer::TcpServer(const std::string& address, uint16_t port)
{
    std::cout << "Server socket created!" << std::endl;
    std::cout << "Binding server socket to address : " << address << ":" << port << "..." << std::endl;

    serverSocket.Bind(address, port);

    std::cout << "Server socket bound!" << std::endl;
}

void TcpServer::Run()
{
    serverSocket.Listen();

    std::cout << "Listening started, waiting for client..." << std::endl;

    Thread acceptClientsThread([this]()
    {
        AcceptClients();
    });

    while (true)
    {
        auto serverEvent = WaitForEvent();

        switch (serverEvent.eventType)
        {
            case TcpServerEvent::Type::eClientAccepted:
            {
                auto disconnectCb = [this](const std::thread::id threadId)
                {
                    { // begin critical section
                        std::lock_guard lock(eventsMutex);
                        serverEvents.push(TcpServerEvent::CreateClientDisconnected(threadId));
                    } // end critical section

                    eventsCondition.notify_one();
                };

                auto clientConnection = std::make_unique<ClientConnection>(std::move(serverEvent.clientSocket.value()), std::move(disconnectCb));
                clientConnections[clientConnection->GetThreadId()] = std::move(clientConnection);
            }
            break;

            case TcpServerEvent::Type::eClientDisconnected:
            {
                auto it = clientConnections.find(serverEvent.threadId.value());

                if (it != clientConnections.end())
                    clientConnections.erase(it);
            }
            break;

            case TcpServerEvent::Type::eShutdownRequested:
            {
                shutdownRequested.store(true);
                serverSocket.Shutdown();

                for (auto& [threadId, connection] : clientConnections)
                    connection->Stop();

                clientConnections.clear();

                return;
            }
            break;
        }
    }
}

void TcpServer::AcceptClients()
{
    while (!shutdownRequested.load())
    {
        std::optional<Socket> client;

        try
        {
            client = serverSocket.Accept();
        }
        catch (const std::exception& e)
        {
            if (shutdownRequested.load())
                break;

            std::cout << "Error during client connection: " << e.what() << std::endl;
            break;
        }

        if (shutdownRequested.load())
            return;

        std::cout << "Client connected!" << std::endl;

        { // begin critical section
            std::lock_guard lock(eventsMutex);
            serverEvents.push(TcpServerEvent::CreateClientAccepted(std::move(client.value())));
        } // end critical section

        eventsCondition.notify_one();
    }
}

TcpServerEvent TcpServer::WaitForEvent()
{
    std::unique_lock lock(eventsMutex);

    while (serverEvents.empty())
    {
        eventsCondition.wait(lock);
    }

    TcpServerEvent serverEvent = std::move(serverEvents.front());
    serverEvents.pop();

    return serverEvent;
}

void TcpServer::Stop()
{
    { // begin critical section
        std::lock_guard lock(eventsMutex);
        serverEvents.push(TcpServerEvent::CreateShutdownRequested());
    } // end critical section

    eventsCondition.notify_one();
}

}
