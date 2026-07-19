#include "Client.hpp"
#include <iostream>

Client::Client(const std::string &serverIP, unsigned short port, hive::PlayerColor player)
    : player(player), serverIP(serverIP), port(port)
{
}

bool Client::startConnection()
{
    if (socket.connect(serverIP, port) != sf::Socket::Done)
    {
        std::cerr << "Error: Could not connect to server at " << serverIP << ":" << port << '\n';
        return false;
    }
    socket.setBlocking(false);

    std::cout << "Connected to server!\n";

    return sendMessage(std::string(1, hive::toChar(player)));
}

bool Client::sendMessage(const std::string &message)
{
    if (socket.send(message.c_str(), message.size()) != sf::Socket::Done)
    {
        std::cerr << "Error: Could not send message to server\n";
        return false;
    }
    return true;
}

bool Client::receiveMessage(std::string &message)
{
    std::size_t received;

    if (socket.receive(buffer, sizeof(buffer), received) == sf::Socket::Done)
    {
        message = std::string(buffer, received);
        return true;
    }

    return false;
}

bool Client::isConnected() const { return socket.getRemoteAddress() != sf::IpAddress::None; }

void Client::run()
{
    startConnection();
    prepare();

    while (!gameStarted && !shouldStop())
    {
        handleGameStart();
    }

    if (shouldStop())
    {
        return;
    }

    std::cout << "Game started!\n";

    while (!shouldStop())
    {
        sf::Time elapsed = clock.restart();

        handleMyAction();
        handleOpponentAction();

        if (elapsed < frameTime)
        {
            sf::sleep(frameTime - elapsed);
        }
    }
}

void Client::handleGameStart()
{
    if (receiveMessage(message))
    {
        if (message == "START")
        {
            gameStarted = true;
        }
    }
}

void parseArguments(hive::PlayerColor &player, int &port, char *argv[])
{
    port = std::stoi(argv[1]);
    player = hive::playerColorFromChar(argv[2][0]);
}
