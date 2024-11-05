#include "Game.hpp"
#include "Client.hpp"
#include "AiAlgorithm.hpp"
#include "RandomAi.hpp"
#include "AlphaBethaSearch.hpp"
#include <iostream>

using namespace hive;

/**
 * @brief A class for managing an AI client
 *
 * The AIClient class extends the Client class to provide TCP client functionality.
 * It uses a specific algorithm to make AI moves.
 */
class AIClient : public Client
{
public:
    AIClient(const std::string &serverIP, unsigned short port, char player, Game& game, AIAlgorithm& aiAlgorithm)
        : Client(serverIP, port, player), game(game), aiAlgorithm(aiAlgorithm) {}

private:
    Game& game;
    AIAlgorithm& aiAlgorithm;

    void prepare() override
    {
        game.startNewGame();
    }

    void handleMyAction() override
    {
        if (game.getCurrentTurn() == player)
        {
            auto availableActions = game.getAvailableActions();
            if (!availableActions.empty())
            {
                Action action = aiAlgorithm.getNextMove();
                game.applyAction(action);
                std::string actionString = game.getLastAction();

                if (!sendMessage(actionString))
                {
                    std::cerr << "Failed to send message to the server" << std::endl;
                }
            }
        }
    }

    void handleOpponentAction() override
    {
        if (game.getCurrentTurn() != player)
        {
            if (receiveMessage(message))
            {
                game.applyAction(message);
            }
        }
    }
};

int main(int argc, char *argv[])
{
    char player;
    int port;
    
    std::string serverIP = "127.0.0.1";
    if (argc > 2)
    {
        parseArguments(player, port, argc, argv);
    }
    else
    {
        return 1;
    }
    std::unique_ptr<Game> game = std::make_unique<Game>();
    std::unique_ptr<AIAlgorithm> aiAlgorithm = std::make_unique<AlphaBetaAI>(*game);


    AIClient client(serverIP, port, player, *game, *aiAlgorithm);
    client.run();
    return 0;
}