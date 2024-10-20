#include "Game.hpp"
#include "Client.hpp"
#include <iostream>

using namespace hive;
class AIClient : public Client
{
public:
    AIClient(const std::string &serverIP, unsigned short port)
        : Client(serverIP, port) {}

    void run(const std::string &player)
    {
        if (!startConnection(player))
        {
            std::cerr << "Failed to connect to server" << std::endl;
            return;
        }

        game.startNewGame();

        std::string message;
        bool gameStarted = false;
        while (true)
        {
            if(!gameStarted)
            {
                if (receiveMessage(message))
                {   std::cerr << "Received message: " << message << std::endl;
                    if(message == "START")
                    {
                        gameStarted = true;
                    }
                }
                continue;
            }//

            if (game.getCurrentTurn() == player )
            {   auto availableActions = game.getAvailableActions();
                if (!availableActions.empty())
                {
                    Action firstAction = *availableActions.begin();
                    std::string actionString = actionParser.actionToString(firstAction);
                    if (!sendMessage(actionString))
                    {
                        std::cerr << "Failed to send message to the server" << std::endl;
                    }
                    game.applyAction(firstAction);
                }
            }

            if (receiveMessage(message))
            {
                game.applyAction(message);
            }
        }
    }

private:
    hive::Game game;
    ActionParser actionParser;
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

    AIClient client("127.0.0.1", port);
    client.run(player);
    return 0;
}