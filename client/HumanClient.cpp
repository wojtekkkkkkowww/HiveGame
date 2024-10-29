#include <SFML/Graphics.hpp>
#include "GameController.hpp"
#include "Client.hpp"

class HumanClient : public Client
{
public:
    HumanClient(const std::string &serverIP, unsigned short port, char player)
        : Client(serverIP, port, player), window(sf::VideoMode(800, 600), "Hive Game", sf::Style::Default, sf::ContextSettings(0, 0, 8)),
          gameController(window) {}

private:
    sf::RenderWindow window;
    GameController gameController;

    void prepare() override
    {
        gameController.setPlayer(player);
    }

    void handleMyAction() override
    {
        gameController.handleGameControll();

        if (gameController.message != "")
        {
            std::cerr << "sending message: " << gameController.message << std::endl;
            if (!sendMessage(gameController.message))
            {
                std::cerr << "Failed to send message to the server" << std::endl;
            }
            gameController.message = "";
        }

    }

    void handleOpponentAction() override
    {
        if (receiveMessage(message))
        {
            std::cerr << "received message: " << message << std::endl;
            gameController.apllyOpponentAction(message);
        }
    }
};

int main(int argc, char *argv[])
{
    char player;
    int port;
    
    if (argc > 2)
    {
        parseArguments(player, port, argc, argv);
    }
    else
    {
        return 1;
    }

    HumanClient client("127.0.0.1", port,player);
    client.run();
    return 0;
}
