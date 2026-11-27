#include "Socket.h"

#include <stdexcept>
#include <system_error>

namespace networking
{

Socket::Socket() 
{
	socketHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (socketHandle == InvalidSocket)
	{
        throw std::system_error(GetLastSocketError(), std::system_category(), "Failed to create TCP socket");
	}
}

Socket::~Socket()
{
	Close();
}

Socket::Socket(Socket&& b) noexcept
	: socketHandle{b.socketHandle}
{
    b.socketHandle = InvalidSocket;
}

Socket& Socket::operator=(Socket&& b) noexcept
{
	if (this == &b)
		return *this;

	Close();

	socketHandle = b.socketHandle;
    b.socketHandle = InvalidSocket;

	return *this;
}

Socket Socket::Accept()
{
	auto clientSocket = accept(socketHandle, nullptr, nullptr);

    if (clientSocket == InvalidSocket)
	{
        throw std::system_error(GetLastSocketError(), std::system_category(), "accept() failed");
	}

	return Socket(clientSocket);
}

void Socket::Bind(const std::string& ipAddress, uint16_t port)
{
	sockaddr_in serverAddress{};

    const auto addressResult = inet_pton(AF_INET, ipAddress.c_str(), &serverAddress.sin_addr);

	if (addressResult != 1)
	{
		throw std::invalid_argument("Invalid IPv4 address: " + ipAddress);
	}

	serverAddress.sin_port = htons(port);
	serverAddress.sin_family = AF_INET;

    if (bind(socketHandle, reinterpret_cast<const sockaddr*>(&serverAddress), sizeof(serverAddress)) == SocketError)
	{
        throw std::system_error(GetLastSocketError(), std::system_category(), "bind() failed");
	}
}


void Socket::Listen(int backlog)
{
    if (listen(socketHandle, backlog) == SocketError)
	{
        throw std::system_error(GetLastSocketError(), std::system_category(), "listen() failed");
	}
}

void Socket::Connect(const std::string& ipAddress, uint16_t port)
{
	sockaddr_in serverAddress{};

    const auto addressResult = inet_pton(AF_INET, ipAddress.c_str(), &serverAddress.sin_addr);

	if (addressResult != 1)
	{
		throw std::invalid_argument("Invalid IPv4 address: " + ipAddress);
	}

	serverAddress.sin_port = htons(port);
	serverAddress.sin_family = AF_INET;

    if (connect(socketHandle, reinterpret_cast<const sockaddr*>(&serverAddress), sizeof(serverAddress)) == SocketError)
	{
        throw std::system_error(GetLastSocketError(), std::system_category(), "connect() failed");
	}
}

int Socket::Send(const void* data, int size)
{
	const auto sendResult = send(socketHandle, reinterpret_cast<const char*>(data), size, 0);

    if (sendResult == SocketError)
        throw std::system_error(GetLastSocketError(), std::system_category(), "send() failed");

	return sendResult;
}

void Socket::SendAll(const void* data, int size)
{
	if (size < 0)
		throw std::invalid_argument("Invalid send buffer size");

	if (size == 0)
		return;

	if (data == nullptr)
		throw std::invalid_argument("Invalid send buffer data");

	const auto* bytes = static_cast<const char*>(data);
	int bytesSent = 0;

	while (bytesSent < size)
	{
		const int currentBytesSent = Send((bytes + bytesSent), (size - bytesSent));

		if (currentBytesSent < 1)
			throw std::runtime_error("send() made no progress");

		bytesSent += currentBytesSent;
	}
}

int Socket::Recv(void* buffer, int size)
{
	const auto recvResult = recv(socketHandle, reinterpret_cast<char*>(buffer), size, 0);

    if (recvResult == SocketError)
        throw std::system_error(GetLastSocketError(), std::system_category(), "recv() failed");

	return recvResult;
}

bool Socket::RecvAll(void* buffer, int size)
{
	if (size < 0)
		throw std::invalid_argument("Invalid recv buffer size");

	if (size == 0)
        return true;

	if (buffer == nullptr)
		throw std::invalid_argument("Invalid recv buffer");

	auto* bytes = static_cast<char*>(buffer);
	int bytesReceived = 0;

	while (bytesReceived < size)
	{
		const int currentBytesReceived = Recv((bytes + bytesReceived), (size - bytesReceived));

        if (currentBytesReceived == 0)
        {
            if (bytesReceived == 0)
                return false;

            throw std::runtime_error("Connection closed before receiving all data");
        }

		bytesReceived += currentBytesReceived;
	}

    return true;
}

void Socket::Close() noexcept
{
    if (socketHandle != InvalidSocket)
	{
        CloseSocket(socketHandle);
        socketHandle = InvalidSocket;
	}
}

void Socket::Shutdown() noexcept
{
    if (socketHandle != InvalidSocket)
    {
        ShutdownSocket(socketHandle);
    }
}

}
