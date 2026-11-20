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

        constexpr auto message = "Hello Server!";

        clientSocket.Send(static_cast<const void*>(message), std::strlen(message));

    }
    catch (const std::exception& e)
    {
        std::cerr << "Client error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}