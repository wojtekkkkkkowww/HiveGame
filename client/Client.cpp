#include "Client.hpp"
#include <iostream>

Client::Client(const std::string &serverIP, unsigned short port)
    : serverIP(serverIP), port(port) {}

bool Client::startConnection(const std::string &playerColor)
{
    if (socket.connect(serverIP, port) != sf::Socket::Done)
    {
        std::cerr << "Error: Could not connect to server at " << serverIP << ":" << port << std::endl;
        return false;
    }
    socket.setBlocking(false);
    
    std::cout << "Connected to server!" << std::endl;

    return sendMessage(playerColor);
}

bool Client::sendMessage(const std::string &message)
{

    if (socket.send(message.c_str(), message.size()) != sf::Socket::Done)
    {
        std::cerr << "Error: Could not send message to server" << std::endl;
        return false;
    }
    return true;
}

bool Client::receiveMessage(std::string &message)
{

    char buffer[128];
    std::size_t received;

    if (socket.receive(buffer, sizeof(buffer), received) == sf::Socket::Done)
    {
        message = std::string(buffer, received);
        return true;
    }

    return false;
}

bool Client::isConnected() const
{
    return socket.getRemoteAddress() != sf::IpAddress::None;
}
