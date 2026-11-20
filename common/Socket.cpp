#include "Socket.h"

#include <stdexcept>
#include <system_error>

namespace networking
{

Socket::Socket() 
{
	socketHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (socketHandle == INVALID_SOCKET)
	{
		throw std::system_error(WSAGetLastError(), std::system_category(), "Failed to create TCP socket");
	}
}

Socket::~Socket()
{
	Close();
}

Socket::Socket(Socket&& b) noexcept
	: socketHandle{b.socketHandle}
{
	b.socketHandle = INVALID_SOCKET;
}

Socket& Socket::operator=(Socket&& b) noexcept
{
	if (this == &b)
		return *this;

	Close();

	socketHandle = b.socketHandle;
	b.socketHandle = INVALID_SOCKET;

	return *this;
}

Socket Socket::Accept()
{
	auto clientSocket = accept(socketHandle, nullptr, nullptr);

	if (clientSocket == INVALID_SOCKET)
	{
		throw std::system_error(WSAGetLastError(), std::system_category(), "accept() failed");
	}

	return Socket(clientSocket);
}

void Socket::Bind(const std::string& ipAddress, uint16_t port)
{
	sockaddr_in serverAddress{};

	const auto addressResult = InetPtonA(AF_INET, ipAddress.c_str(), &serverAddress.sin_addr);

	if (addressResult != 1)
	{
		throw std::invalid_argument("Invalid IPv4 address: " + ipAddress);
	}

	serverAddress.sin_port = htons(port);
	serverAddress.sin_family = AF_INET;

	if (bind(socketHandle, reinterpret_cast<const sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR)
	{
		throw std::system_error(WSAGetLastError(), std::system_category(), "bind() failed");
	}
}


void Socket::Listen(int backlog)
{
	if (listen(socketHandle, backlog) == SOCKET_ERROR)
	{
		throw std::system_error(WSAGetLastError(), std::system_category(), "listen() failed");
	}
}

void Socket::Connect(const std::string& ipAddress, uint16_t port)
{
	sockaddr_in serverAddress{};

	const auto addressResult = InetPtonA(AF_INET, ipAddress.c_str(), &serverAddress.sin_addr);

	if (addressResult != 1)
	{
		throw std::invalid_argument("Invalid IPv4 address: " + ipAddress);
	}

	serverAddress.sin_port = htons(port);
	serverAddress.sin_family = AF_INET;

	if (connect(socketHandle, reinterpret_cast<const sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR)
	{
		throw std::system_error(WSAGetLastError(), std::system_category(), "connect() failed");
	}
}

int Socket::Send(const void* data, int size)
{
	const auto sendResult = send(socketHandle, reinterpret_cast<const char*>(data), size, 0);

	if (sendResult == SOCKET_ERROR)
		throw std::system_error(WSAGetLastError(), std::system_category(), "send() failed");

	return sendResult;
}

int Socket::Recv(void* buffer, int size)
{
	const auto recvResult = recv(socketHandle, reinterpret_cast<char*>(buffer), size, 0);

	if (recvResult == SOCKET_ERROR)
		throw std::system_error(WSAGetLastError(), std::system_category(), "recv() failed");

	return recvResult;
}

void Socket::Close() noexcept
{
	if (socketHandle != INVALID_SOCKET)
	{
		closesocket(socketHandle);
		socketHandle = INVALID_SOCKET;
	}
}

}