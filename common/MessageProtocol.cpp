#include "MessageProtocol.h"
#include "Socket.h"

#include <cstdint>
#include <stdexcept>
#include <memory>

namespace networking::protocol
{

namespace
{
    constexpr uint32_t MAX_MESSAGE_SIZE = 64 * 1024;
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

std::string ReceiveMessage(Socket& socket)
{
    uint32_t networkLength = 0;
    socket.RecvAll(&networkLength, sizeof(networkLength));

    const uint32_t messageLength = ntohl(networkLength);

    if (messageLength > MAX_MESSAGE_SIZE)
        throw std::invalid_argument("Received message exceeds maximum allowed size!");

    std::string message(messageLength, '\0');

    socket.RecvAll(message.data(), messageLength);

    return message;
}
}
