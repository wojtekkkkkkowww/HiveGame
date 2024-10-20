#pragma once
#include <SFML/Network.hpp>
#include <string>

class Client {
public:
    Client(const std::string& serverIP, unsigned short port);
    bool startConnection(const std::string& playerColor); 
    bool sendMessage(const std::string& message);         
    bool receiveMessage(std::string& message);            
    bool isConnected() const;                             

private:
    sf::TcpSocket socket;  
    std::string serverIP;  
    unsigned short port;   
};
