#include <iostream>
#include <string>
#include <exception>

#include "Socket.h"
#include "NetworkRuntime.h"
#include "MessageProtocol.h"

int main()
{
    try
    {
        std::cout << "Initializing Winsock..." << std::endl;

        networking::NetworkRuntime winsock;

        std::cout << "Winsock initialized successfully!" << std::endl;

        networking::Socket clientSocket;

        std::cout << "Client socket created!" << std::endl;

        std::cout << "Connecting client socket to 127.0.0.1:9000..." << std::endl;

        clientSocket.Connect("127.0.0.1", 9000);

        std::cout << "Client connected!" << std::endl;

        constexpr std::string_view messages[] = {"Hello Server!", "Today is friday", "Have a nice day!"};

        for (const auto& message : messages)
        {
            networking::protocol::SendMessage(clientSocket, message);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Client error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}