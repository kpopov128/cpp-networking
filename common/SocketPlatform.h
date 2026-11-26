#pragma once

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>
#endif

namespace networking
{

#ifdef _WIN32

using SocketHandle = SOCKET;

inline constexpr SocketHandle InvalidSocket = INVALID_SOCKET;
inline constexpr int SocketError = SOCKET_ERROR;
inline constexpr int SendFlags = 0;

inline int GetLastSocketError() noexcept
{
    return WSAGetLastError();
}

inline void CloseSocket(SocketHandle socket) noexcept
{
    closesocket(socket);
}

#else

using SocketHandle = int;

inline constexpr SocketHandle InvalidSocket = -1;
inline constexpr int SocketError = -1;
inline constexpr int SendFlags = MSG_NOSIGNAL;

inline int GetLastSocketError() noexcept
{
    return errno;
}

inline void CloseSocket(SocketHandle socket) noexcept
{
    close(socket);
}

#endif // #ifdef _WIN32

}
