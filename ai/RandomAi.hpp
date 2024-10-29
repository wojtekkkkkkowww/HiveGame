#pragma once
#include "AiAlgorithm.hpp"

namespace hive
{
    class RandomAIAlgorithm : public AIAlgorithm
    {
    public:
        RandomAIAlgorithm(Game& game);
        Action getNextMove() override;
    };
}

