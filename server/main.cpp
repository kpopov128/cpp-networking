
#include <iostream>
#include <string>
#include <exception>

#include "TcpServer.h"

namespace 
{
    constexpr size_t MAX_BUFFER_SIZE = (64 * 1024);
}

int main()
{
    networking::TcpServer server("127.0.0.1", 9000);

    server.Run();

    return 0;
}
