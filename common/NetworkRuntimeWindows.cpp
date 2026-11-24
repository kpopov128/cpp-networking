#include "NetworkRuntime.h"

#include <winsock2.h>
#include <system_error>
#include <stdexcept>
#include <iostream>

namespace networking
{

NetworkRuntime::NetworkRuntime()
{
    WSADATA data{};

    std::cout << "Initializing Winsock..." << std::endl;

    const int result = WSAStartup(WINSOCK_VERSION, &data);

    if (result != 0)
    {
        throw std::system_error(result, std::system_category(), "WSAStartup() failed");
    }

    if (data.wVersion != WINSOCK_VERSION)
    {
        WSACleanup();

        throw std::runtime_error("Unsupported Winsock version");
    }

    std::cout << "Winsock initialized successfully!" << std::endl;
}

NetworkRuntime::~NetworkRuntime()
{
    WSACleanup();
}

}
