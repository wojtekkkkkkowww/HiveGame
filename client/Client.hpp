#pragma once
#include "Game.hpp"
#include <SFML/Network.hpp>
#include <string>

/**
 * @brief A class for managing a TCP client connection.
 *
 * The Client class provides methods to connect to a server, send and receive messages,
 * and check the connection status. It uses the SFML library for networking.
 */
class Client
{
  public:
    Client(const std::string &serverIP, unsigned short port, hive::PlayerColor player);

    /**
     * @brief Main loop of the client
     *
     * The run method starts the connection to the server and then enters the main loop.
     * In the main loop, it handles making actions and receiving opponent actions.
     */
    void run();

  protected:
    virtual void handleMyAction() = 0;
    virtual void handleOpponentAction() = 0;
    virtual void prepare() = 0;

    virtual bool shouldStop() const { return false; }

    bool sendMessage(const std::string &message);
    bool receiveMessage(std::string &message);

    std::string message;
    hive::PlayerColor player;
    bool gameStarted = false;

  private:
    void handleGameStart();
    bool startConnection();
    bool isConnected() const;

    sf::TcpSocket socket;
    std::string serverIP;
    unsigned short port;
    sf::Clock clock;
    const sf::Time frameTime = sf::seconds(1.f / 20.f);
    char buffer[128];
};

void parseArguments(hive::PlayerColor &player, int &port, char *argv[]);
