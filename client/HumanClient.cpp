#include <SFML/Graphics.hpp>
#include "GameController.hpp"
#include "Client.hpp"

class HumanClient : public Client
{
public:
    HumanClient(const std::string &serverIP, unsigned short port)
        : Client(serverIP, port), window(sf::VideoMode(800, 600), "Hive Game", sf::Style::Default, sf::ContextSettings(0, 0, 8)),
          gameController(window) {}

    void run(const std::string &player)
    {
        if (!startConnection(player))
        {
            std::cerr << "Failed to connect to server" << std::endl;
            return;
        }
        gameController.setPlayer(player);

        std::string message;
        sf::Clock clock;
        const sf::Time frameTime = sf::seconds(1.f / 20.f);
        bool gameStarted = false;
        while (window.isOpen())
        {
            if (!gameStarted)
            {
                if (receiveMessage(message))
                {
                    if (message == "START")
                    {
                        gameStarted = true;
                    }
                }
                continue;
            }

            sf::Time elapsed = clock.restart();
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

            if (receiveMessage(message))
            {
                std::cerr << "recived message: " << message << std::endl;
                gameController.apllyOpponentAction(message);
            }

            if (elapsed < frameTime)
            {
                sf::sleep(frameTime - elapsed);
            }
        }
    }

    sf::RenderWindow window;
    GameController gameController;
};

int main(int argc, char *argv[])
{
    std::string player;
    int port;
    if (argc > 2)
    {
        port = std::stoi(argv[1]);
        player = argv[2];

        if (player != "WHITE" && player != "BLACK")
        {
            std::cerr << "Invalid player color" << std::endl;
            return 1;
        }
    }
    else
    {
        return 1;
    }

    HumanClient client("127.0.0.1", port);
    client.run(player);
    return 0;
}

// QSGBA
// example move message:
// M0110
// example place message:
// PQ10

// void GameController::run()
// {
//     sf::Clock clock;
//     const sf::Time frameTime = sf::seconds(1.f / 20.f);

//     while (window.isOpen())
//     {
//         sf::Time elapsed = clock.restart();
//         handleGameControll();

//         if (elapsed < frameTime)
//         {
//             sf::sleep(frameTime - elapsed);
//         }
//     }
// }