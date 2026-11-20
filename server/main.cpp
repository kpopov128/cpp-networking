
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

        networking::Socket serverSocket;

        std::cout << "Server socket created!" << std::endl;
        std::cout << "Binding server socket to 127.0.0.1:9000..." << std::endl;

        serverSocket.Bind("127.0.0.1", 9000);

        std::cout << "Server socket bound!" << std::endl;

        serverSocket.Listen();

        std::cout << "Listening started, waiting for client..." << std::endl;

        networking::Socket clientSocket = serverSocket.Accept();

        std::cout << "Client connected!" << std::endl;

        const int clientBufferSize = 1024;
        char buffer[clientBufferSize];

        const int bytesReceived = clientSocket.Recv(static_cast<void*>(buffer), sizeof(buffer));

        if (bytesReceived > 0)
        {
            std::string message(buffer, bytesReceived);
            std::cout << "Received: " << message << std::endl;
        }
        else if (bytesReceived == 0)
        {
            std::cout << "Client disconnected." << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Server error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
