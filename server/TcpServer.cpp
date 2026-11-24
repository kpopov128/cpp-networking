#include "TcpServer.h"
#include "MessageProtocol.h"

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

    while (true)
    {
        HandleClient(serverSocket.Accept());
    }
}

void TcpServer::HandleClient(Socket clientSocket)
{
    std::cout << "Client connected!" << std::endl;

    for (int i = 0; i < 3; ++i)
    {
        std::cout << "Message received -> " << networking::protocol::ReceiveMessage(clientSocket) << std::endl;
    }   
}

}
