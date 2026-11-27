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

    TcpServerEvent serverEvent;
    while (true)
    {
        { // begin critical section
            std::unique_lock lock(eventsMutex);

            while (serverEvents.empty())
            {
                eventsCondition.wait(lock);
            }

            serverEvent = std::move(serverEvents.front());
            serverEvents.pop();
        } // end critical section

        switch(serverEvent.eventType)
        {
            case TcpServerEvent::Type::eClientAccepted:
            {
                auto disconnectCb = [this](const std::thread::id threadId)
                {
                    { // begin critical section
                        std::lock_guard lock(eventsMutex);
                        serverEvents.push(TcpServerEvent(threadId));
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
        }
    }
}

void TcpServer::AcceptClients()
{
    while (true)
    {
        auto client = serverSocket.Accept();
        std::cout << "Client connected!" << std::endl;

        { // begin critical section
            std::lock_guard lock(eventsMutex);
            serverEvents.push(TcpServerEvent(std::move(client)));
        } // end critical section

        eventsCondition.notify_one();
    }
}
}
