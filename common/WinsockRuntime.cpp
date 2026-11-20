#include "WinsockRuntime.h"

#include <winsock2.h>
#include <system_error>
#include <stdexcept>

namespace networking
{

WinsockRuntime::WinsockRuntime()
{
    WSADATA data{};

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
}

WinsockRuntime::~WinsockRuntime()
{
    WSACleanup();
}

}
