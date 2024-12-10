#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <set>

constexpr auto END_MESSAGES = {"WHITE_WINS", "BLACK_WINS", "DRAW"};
constexpr size_t bufferSize = 128;
char buffer[bufferSize];

bool isGameOver(const std::string &message)
{
    for (const auto &endMsg : END_MESSAGES)
    {
        if (message == endMsg)
        {
            return true;
        }
    }
    return false;
}

void startConnection(sf::TcpListener &listener, int PORT)
{
    if (listener.listen(PORT) != sf::Socket::Done)
    {
        throw std::runtime_error("Could not start listening on port" + std::to_string(PORT));
    }
    std::cout << "Waiting for two players to connect..." << std::endl;
}

std::string receiveMessage(sf::TcpSocket &player)
{
    std::size_t received;
    if (player.receive(buffer, sizeof(buffer), received) != sf::Socket::Done)
    {
        throw std::runtime_error("Could not receive message from player");
    }
    return std::string(buffer, received);
}

void sendMessage(sf::TcpSocket &player, const std::string &message)
{
    if (player.send(message.c_str(), message.size()) != sf::Socket::Done)
    {
        throw std::runtime_error("Could not send message to player");
    }
}

void connectPlayer(sf::TcpListener &listener, sf::TcpSocket &player, const std::string &prompt)
{
    if (listener.accept(player) != sf::Socket::Done)
    {
        throw std::runtime_error("Could not accept player");
    }
    std::cout << prompt << std::endl;
}

std::string chooseColor(sf::TcpSocket &player)
{
    std::size_t received;
    std::cout << "Please send your color (WHITE or BLACK):" << std::endl;

    if (player.receive(buffer, bufferSize, received) != sf::Socket::Done)
    {
        throw std::runtime_error("Could not receive color from player");
    }

    return std::string(buffer, received);
}

void startGame(sf::TcpSocket &player1, sf::TcpSocket &player2)
{
    std::string startMessage = "START";
    if (player1.send(startMessage.c_str(), startMessage.size()) != sf::Socket::Done)
    {
        throw std::runtime_error("Could not send start message to player 1");
    }

    if (player2.send(startMessage.c_str(), startMessage.size()) != sf::Socket::Done)
    {
        throw std::runtime_error("Could not send start message to player 2");
    }
    std::cout << "Game started! Players take turns sending messages..." << std::endl;
}

int main(int argc, char *argv[])
{
    int PORT = 54000;
    if (argc > 1)
    {
        PORT = std::stoi(argv[1]);
    }

    sf::TcpListener listener;
    startConnection(listener, PORT);

    sf::TcpSocket player1, player2;
    std::string player1Color;
    std::string player2Color;

    connectPlayer(listener, player1, "Player 1 connected!");
    player1Color = chooseColor(player1);
    std::cout << "Player 1 chose color: " << player1Color << std::endl;
    connectPlayer(listener, player2, "Player 2 connected!");

    while (true)
    {
        player2Color = chooseColor(player2);

        if (player2Color == player1Color)
        {
            std::string errorMsg = "Color already chosen! Please choose a different color.";
            sendMessage(player2, errorMsg);
            std::cout << "Player 2 tried to choose duplicate color: " << player2Color << std::endl;
            std::cout << "Waiting for player 2 to choose a different color..." << std::endl;
        }
        else
        {
            std::cout << "Player 2 chose color: " << player2Color << std::endl;
            break;
        }
    }

    sf::TcpSocket *currentPlayer = &player1;
    sf::TcpSocket *waitingPlayer = &player2;

    startGame(player1, player2);

    bool gameRunning = true;
    while (gameRunning)
    {
        std::string message = receiveMessage(*currentPlayer);
        std::cout << "Received message: " << message << std::endl;

        if (isGameOver(message))
        {
            std::cout << "Game over! " << message << std::endl;
            gameRunning = false;
        }

        sendMessage(*waitingPlayer, message);
        std::swap(currentPlayer, waitingPlayer);
    }

    std::cout << "Shutting down server..." << std::endl;

    player1.disconnect();
    player2.disconnect();

    return 0;
}