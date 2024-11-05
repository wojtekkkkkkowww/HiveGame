#include "RandomAi.hpp"
#include <cstdlib>
#include <ctime>
#include <iterator>

//URZYJ mesuretwister ? z randoma
namespace hive
{
    RandomAIAlgorithm::RandomAIAlgorithm(Game& game) : AIAlgorithm(game, "Random AI")
    {
        std::srand(std::time(nullptr)); // Seed for random number generation
    }

    Action RandomAIAlgorithm::getNextMove()
    {
        auto availableActions = game.getAvailableActions();
       // std::cerr << "Available actions: " << availableActions.size() << std::endl;
        if (!availableActions.empty())
        {
            try{
                auto it = availableActions.begin();
                std::advance(it, std::rand() % availableActions.size());
                return *it;
            }catch(std::exception e){
                std::cerr << "sigma \n";
            } 
        }
        //std::cerr << "No available actions" << std::endl;
        return Action(); // Return a default action if no actions are available
    }
}

//"PAMIETAJ O TRY CATCH"