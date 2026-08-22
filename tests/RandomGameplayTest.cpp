#include "Game.hpp"
#include <gtest/gtest.h>
#include <random>
#include <stdexcept>

using namespace hive;

TEST(RandomGameplayTest, PlaysRandomLegalGamesWithoutInvalidTransitions)
{
    constexpr std::size_t gameCount = 250;
    std::mt19937 randomGenerator(0xC0FFEEu);

    for (std::size_t gameIndex = 0; gameIndex < gameCount; ++gameIndex)
    {
        Game game;
        EXPECT_THROW(game.getLastAction(), std::logic_error);

        std::size_t playedPly = 0;
        while (!game.isGameOver())
        {
            const auto &availableActions = game.getAvailableActions();
            ASSERT_FALSE(availableActions.empty()) << "No legal action in game " << gameIndex;

            std::uniform_int_distribution<std::size_t> distribution(0, availableActions.size() - 1);
            const Action action = availableActions[distribution(randomGenerator)];

            ASSERT_TRUE(game.applyAction(action)) << "Rejected supposedly legal action in game "
                                                  << gameIndex;
            const auto lastAction = game.getLastAction();
            ASSERT_FALSE(lastAction.empty());

            ++playedPly;
            ASSERT_LE(playedPly, game.getMaxPly()) << "Game exceeded configured ply limit";
        }

        game.startNewGame();
        EXPECT_THROW(game.getLastAction(), std::logic_error);
    }
}
