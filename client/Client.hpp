#pragma once
#include <SFML/Network.hpp>
#include <string>
#include "Game.hpp"

/**
 * @brief A class for managing a TCP client connection.
 * 
 * The Client class provides methods to connect to a server, send and receive messages,
 * and check the connection status. It uses the SFML library for networking.
 */
class Client {
public:
    Client(const std::string& serverIP, unsigned short port, char player);
    
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
    bool sendMessage(const std::string& message);         
    bool receiveMessage(std::string& message);            
    
    std::string message; ///< The message buffer for communication.
    char player; 
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

};

void parseArguments(char &player, int &port, int argc, char *argv[]);
