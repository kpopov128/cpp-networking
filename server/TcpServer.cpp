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
        Socket clientSocket;
        { // begin critical section
            std::lock_guard lock(acceptedClientsMutex);
            if (acceptedClients.empty())
                continue;

            clientSocket = std::move(acceptedClients.front());
            acceptedClients.pop();
        } // end critical section

        auto clientConnection = std::make_unique<ClientConnection>(std::move(clientSocket));
        clientConnections[clientConnection->GetThreadId()] = std::move(clientConnection);
    }
}

void TcpServer::AcceptClients()
{
    while (true)
    {
        auto client = serverSocket.Accept();
        std::cout << "Client connected!" << std::endl;

        std::lock_guard lock(acceptedClientsMutex);
        acceptedClients.push(std::move(client));
    }
}

}
