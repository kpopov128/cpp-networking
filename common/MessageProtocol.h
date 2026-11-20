#pragma once

#include <string>
#include <string_view>

namespace networking
{
class Socket;

namespace protocol
{

	void SendMessage(Socket& socket, std::string_view message);
	std::string ReceiveMessage(Socket& socket);

}}
