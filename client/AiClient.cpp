#include "AiAlgorithm.hpp"
#include "AlphaBethaSearch.hpp"
#include "Client.hpp"
#include "Game.hpp"
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
    AIClient(const std::string &serverIP, unsigned short port, PlayerColor player, Game &game,
             AIAlgorithm &aiAlgorithm)
        : Client(serverIP, port, player), game(game), aiAlgorithm(aiAlgorithm)
    {
    }

  private:
    Game &game;
    AIAlgorithm &aiAlgorithm;

    void prepare() override { game.startNewGame(); }

    void handleMyAction() override
    {
        if (game.getCurrentTurn() == player)
        {
            if (!game.getAvailableActions().empty())
            {
                Action action = aiAlgorithm.getNextMove();
                if (!game.applyAction(action))
                {
                    std::cerr << "AI generated invalid action: " << action << '\n';
                    return;
                }
                std::string actionString = game.getLastAction();

                if (!sendMessage(actionString))
                {
                    std::cerr << "Failed to send message to the server \n";
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
                if (const auto result = game.applyActionExpected(message); !result)
                {
                    std::cerr << "Failed to apply opponent action: " << message
                              << " reason: " << toString(result.error()) << '\n';
                }
            }
        }
    }
};

int main(int argc, char *argv[])
{
    PlayerColor player{PlayerColor::White};
    int port{54000};

    std::string serverIP = "127.0.0.1";
    if (argc > 2)
    {
        parseArguments(player, port, argv);
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
