#include <iostream>
#include <string>
#include <exception>

#include "Socket.h"
#include "WinsockRuntime.h"

int main()
{
    try
    {
        std::cout << "Initializing Winsock..." << std::endl;

        networking::WinsockRuntime winsock;

        std::cout << "Winsock initialized successfully!" << std::endl;

        networking::Socket clientSocket;

        std::cout << "Client socket created!" << std::endl;

        std::cout << "Connecting client socket to 127.0.0.1:9000..." << std::endl;

        clientSocket.Connect("127.0.0.1", 9000);

        std::cout << "Client connected!" << std::endl;

        constexpr std::string_view messages[] = {"Hello Server!", "Today is friday", "Have a nice day!"};

        for (const auto& message : messages)
        {
            const uint32_t messageLength = static_cast<uint32_t>(message.size());
            const uint32_t networkMessageLength = htonl(messageLength);

            clientSocket.SendAll(&networkMessageLength, sizeof(networkMessageLength));
            clientSocket.SendAll(message.data(), messageLength);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Client error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}