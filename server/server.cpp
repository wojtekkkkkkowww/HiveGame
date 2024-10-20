#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <set>

constexpr auto END_MESSAGES = {"WHITE_WINS", "BLACK_WINS", "DRAW"};

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

int main(int argc, char *argv[])
{
    int PORT = 54000;
    if (argc > 1)
    {
        PORT = std::stoi(argv[1]);
    }

    sf::TcpListener listener;
    if (listener.listen(PORT) != sf::Socket::Done)
    {
        std::cerr << "Error: Could not start listening on port " << PORT << std::endl;
        return -1;
    }

    std::cout << "Waiting for two players to connect..." << std::endl;

    sf::TcpSocket player1, player2;

    if (listener.accept(player1) != sf::Socket::Done)
    {
        std::cerr << "Error: Could not accept player 1" << std::endl;
        return -1;
    }
    std::cout << "Player 1 connected! Please send your color (WHITE or BLACK):" << std::endl;

    std::string player1Color;
    char buffer[128];
    std::size_t received;

    if (player1.receive(buffer, sizeof(buffer), received) != sf::Socket::Done)
    {
        std::cerr << "Error: Could not receive color from player 1" << std::endl;
        return -1;
    }
    player1Color = std::string(buffer, received);
    std::cout << "Player 1 chose color: " << player1Color << std::endl;

    if (listener.accept(player2) != sf::Socket::Done)
    {
        std::cerr << "Error: Could not accept player 2" << std::endl;
        return -1;
    }
    std::cout << "Player 2 connected! Please send your color (WHITE or BLACK):" << std::endl;

    std::string player2Color;

    while (true)
    {
        if (player2.receive(buffer, sizeof(buffer), received) != sf::Socket::Done)
        {
            std::cerr << "Error: Could not receive color from player 2" << std::endl;
            return -1;
        }
        player2Color = std::string(buffer, received);

        if (player2Color == player1Color)
        {
            std::string errorMsg = "Color already chosen! Please choose a different color.";
            player2.send(errorMsg.c_str(), errorMsg.size());
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

    std::string startMessage = "START";
    if (player1.send(startMessage.c_str(), startMessage.size()) != sf::Socket::Done)
    {
        std::cerr << "Error: Could not send start message to player 1" << std::endl;
        return -1;
    }

    if (player2.send(startMessage.c_str(), startMessage.size()) != sf::Socket::Done)
    {
        std::cerr << "Error: Could not send start message to player 2" << std::endl;
        return -1;
    }
    std::cout << "Game started! Players take turns sending messages..." << std::endl;

    bool gameRunning = true;
    while (gameRunning)
    {

        if (currentPlayer->receive(buffer, sizeof(buffer), received) != sf::Socket::Done)
        {
            std::cerr << "Error: Could not receive message from player" << std::endl;
            return -1;
        }

        std::string message(buffer, received);
        std::cout << "Received message: " << message << std::endl;

        if (isGameOver(message))
        {
            std::cout << "Game over! " << message << std::endl;
            gameRunning = false;
        }

        if (waitingPlayer->send(message.c_str(), message.size()) != sf::Socket::Done)
        {
            std::cerr << "Error: Could not send message to waiting player" << std::endl;
            return -1;
        }

        std::swap(currentPlayer, waitingPlayer);
    }

    std::cout << "Shutting down server..." << std::endl;

    player1.disconnect();
    player2.disconnect();

    return 0;
}
