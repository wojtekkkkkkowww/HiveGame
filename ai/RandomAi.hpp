#pragma once
#include "AiAlgorithm.hpp"
#include <random>


namespace hive
{
    class RandomAIAlgorithm : public AIAlgorithm
    {
    public:
        RandomAIAlgorithm(Game &game);
        Action getNextMove() override;
        std::mt19937 randomEngine;
    
    private:
        std::optional<Action> winInOneMove(char player);
    };
}
