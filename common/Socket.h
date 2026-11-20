#pragma once 

#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

namespace networking
{

class Socket
{
public:
	Socket();
	Socket(SOCKET socket) : socketHandle{socket} {}
	Socket(Socket&& b) noexcept;
	Socket(const Socket& b) = delete;
	~Socket();

	Socket& operator=(const Socket& b) = delete;
	Socket& operator=(Socket&& b) noexcept;

	Socket Accept();
	void Bind(const std::string& ipAddress, uint16_t port);
	void Close() noexcept;
	void Connect(const std::string& ipAddress, uint16_t port);
	void Listen(int backlog = SOMAXCONN);

	int Send(const void* data, int size);
	int Recv(void* buffer, int size);

private:
	SOCKET socketHandle{INVALID_SOCKET};
};

}