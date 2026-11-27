
#include <iostream>
#include <string>
#include <exception>

#include "TcpServer.h"
#include "ShutdownHandler.h"

namespace 
{
    constexpr size_t MAX_BUFFER_SIZE = (64 * 1024);
}

int main()
{
    networking::TcpServer server("127.0.0.1", 9000);

    networking::ShutdownHandler shutdownHandler([&]{server.Stop();});

    server.Run();

    std::cout << "Server stoped!" << std::endl;

    return 0;
}
