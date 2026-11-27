#pragma once

#include "SocketPlatform.h"

#include <string>

namespace networking
{

class Socket
{
public:
	Socket();
    Socket(SocketHandle socket) : socketHandle{socket} {}
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
    void Shutdown() noexcept;

	int Send(const void* data, int size);
	void SendAll(const void* data, int size);

	int Recv(void* buffer, int size);
    bool RecvAll(void* buffer, int size);

private:
    SocketHandle socketHandle{InvalidSocket};
};

}
