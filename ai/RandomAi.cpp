#include "RandomAi.hpp"
#include <cstdlib>
#include <ctime>
#include <iterator>

// URZYJ mesuretwister ? z randoma
namespace hive
{
    RandomAIAlgorithm::RandomAIAlgorithm(Game &game) : AIAlgorithm(game, "Random AI")
    {
        std::srand(std::time(nullptr)); // Seed for random number generation
    }

    Action RandomAIAlgorithm::getNextMove()
    {
        auto availableActions = game.getAvailableActions();
        // std::cerr << "Available actions: " << availableActions.size() << std::endl;

        //win in one move
        std::string player = game.getCurrentTurn() == 'W' ? "WHITE" : "BLACK";
        for (const auto &action : availableActions)
        {
            if (action.type == "PLACE")
                continue;

            game.applyValidAction(action);
            if (game.isGameOver() && game.getGameStatus() == player + "_WINS")
            {
                std::cout << "bardzo ciekawe: " + game.getGameStatus() << std::endl;
                game.revertAction(availableActions);
                return action;
            }
            game.revertAction(availableActions);
        }

        if (!availableActions.empty())
        {
            auto it = availableActions.begin();
            std::advance(it, std::rand() % availableActions.size());
            return *it;
        }
        // std::cerr << "No available actions" << std::endl;
        return Action(); // Return a default action if no actions are available
    }
}

//"PAMIETAJ O TRY CATCH"