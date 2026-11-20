
#include <iostream>
#include <string>
#include <exception>

#include "Socket.h"
#include "WinsockRuntime.h"

namespace 
{
    constexpr size_t MAX_BUFFER_SIZE = (64 * 1024);
}

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

        for (int i = 0; i < 3; ++i)
        {
            uint32_t networkLength = 0;
            clientSocket.RecvAll(&networkLength, sizeof(networkLength));

            const uint32_t clientBufferSize = ntohl(networkLength);

            if (clientBufferSize > MAX_BUFFER_SIZE)
                throw std::invalid_argument("Invalid recv request buffer size from client!");

            auto recvBuffer = std::make_unique<char[]>(clientBufferSize);

            clientSocket.RecvAll(recvBuffer.get(), clientBufferSize);

            std::cout << "Received: " << std::string(recvBuffer.get(), clientBufferSize) << std::endl;
        }
      
    }
    catch (const std::exception& e)
    {
        std::cerr << "Server error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
