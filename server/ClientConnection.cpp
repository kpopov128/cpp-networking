#include "ClientConnection.h"
#include "MessageProtocol.h"

#include <exception>
#include <iostream>
#include <utility>


namespace networking
{

ClientConnection::ClientConnection(Socket&& socket, std::function<void(const std::thread::id)>&& disconnectedCb)
    : clientSocket{std::move(socket)}, clientDisconnectedCb{std::move(disconnectedCb)}, clientThread{[this]{HandleConnection();}}
{

}

void ClientConnection::HandleConnection()
{
    try
    {
        while (true)
        {
            const auto clientMessage = networking::protocol::ReceiveMessage(clientSocket);

            if (!clientMessage)
            {
                std::cout << "Client disconnected..." << std::endl;
                break;
            }

            std::cout << "Client message received: " << clientMessage.value() << std::endl;

            std::cout << "Replying to client with: " << clientMessage.value() << std::endl;
            networking::protocol::SendMessage(clientSocket, clientMessage.value());
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Client communication error: " << e.what() << std::endl;
    }

    clientDisconnectedCb(std::this_thread::get_id());
}

} // namespace networking
