#pragma once

#include <string>
#include <string_view>
#include <optional>

namespace networking
{
class Socket;

namespace protocol
{

	void SendMessage(Socket& socket, std::string_view message);
    std::optional<std::string> ReceiveMessage(Socket& socket);

}}
