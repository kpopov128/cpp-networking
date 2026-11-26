#include <iostream>
#include <string>
#include <exception>

#include "Socket.h"
#include "TcpClient.h"
#include "NetworkRuntime.h"
#include "MessageProtocol.h"

int main()
{
    try
    {
        std::cout << "Initializing NetworkRuntime..." << std::endl;

        networking::NetworkRuntime neworkRuntime;

        std::cout << "NetworkRuntime initialized successfully!" << std::endl;

        networking::TcpClient client("127.0.0.1", 9000); //,

        std::cout << "Client socket created!" << std::endl;

        std::cout << "Connecting client socket to 127.0.0.1:9000..." << std::endl;

        client.Connect();

        std::cout << "Client connected!" << std::endl;

        while (true)
        {
            std::cout << "Enter the message for the server: ";
            std::string message;

            if (!std::getline(std::cin, message) || message == "exit")
                break;

            client.SendMessage(message);

            const auto& serverReply = client.ReceiveMessage();

            if (!serverReply)
            {
                std::cout << "Server disconnected!\n Exiting..." << std::endl;
                break;
            }

            std::cout << "Received message from the server: " << serverReply.value() << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Client error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
