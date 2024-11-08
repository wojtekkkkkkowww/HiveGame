#include "Client.hpp"
#include <iostream>

Client::Client(const std::string &serverIP, unsigned short port, char player)
    : player(player), serverIP(serverIP), port(port) {}

bool Client::startConnection()
{
    if (socket.connect(serverIP, port) != sf::Socket::Done)
    {
        std::cerr << "Error: Could not connect to server at " << serverIP << ":" << port << std::endl;
        return false;
    }
    socket.setBlocking(false);

    std::cout << "Connected to server!" << std::endl;

    return sendMessage(std::string(1, player));
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

void Client::run()
{
    startConnection();
    prepare();

    while (!gameStarted)
    {
        handleGameStart();
    }
    std::cout << "Game started!" << std::endl;

    while (true)
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

void parseArguments(char &player, int &port, char *argv[])
{
    port = std::stoi(argv[1]);
    player = argv[2][0];

    if (player != 'W' && player != 'B')
    {
        std::cerr << "Invalid player color" << std::endl;
        throw std::invalid_argument("Invalid player color");
    }
}
