#include "TcpClient.h"
#include "MessageProtocol.h"

namespace networking
{

TcpClient::TcpClient(const std::string& address, uint16_t port)
    : serverAddress{address}, serverPort{port}
{

}

void TcpClient::Connect()
{
    clientSocket.Connect(serverAddress, serverPort);
}

void TcpClient::SendMessage(const std::string& message)
{
    networking::protocol::SendMessage(clientSocket, message);
}

std::optional<std::string> TcpClient::ReceiveMessage()
{
    return networking::protocol::ReceiveMessage(clientSocket);
}

}
