#include "MessageProtocol.h"
#include "Socket.h"

#include <cstdint>
#include <stdexcept>
#include <limits>

namespace networking::protocol
{

namespace
{
    constexpr uint32_t MAX_MESSAGE_SIZE = 64 * 1024;

    static_assert(MAX_MESSAGE_SIZE <= std::numeric_limits<int>::max(), "MAX_MESSAGE_SIZE exceeds the supported socket buffer size");
}

void SendMessage(Socket& socket, std::string_view message)
{
    if (message.size() > MAX_MESSAGE_SIZE)
        throw std::invalid_argument("Invalid send request buffer size from client!");

    const uint32_t messageLength = static_cast<uint32_t>(message.size());
    const uint32_t networkMessageLength = htonl(messageLength);

    socket.SendAll(&networkMessageLength, sizeof(networkMessageLength));
    socket.SendAll(message.data(), messageLength);
}

std::optional<std::string> ReceiveMessage(Socket& socket)
{
    uint32_t networkLength = 0;

    if (!socket.RecvAll(&networkLength, sizeof(networkLength)))
        return std::nullopt;

    const uint32_t messageLength = ntohl(networkLength);

    if (messageLength > MAX_MESSAGE_SIZE)
        throw std::runtime_error("Received message exceeds maximum allowed size!");

    std::string message(messageLength, '\0');

    if (!socket.RecvAll(message.data(), messageLength))
        throw std::runtime_error("Connection closed before receiving a message, after receiving a header!");

    return message;
}
}
