#include "Game.hpp"
#include "BaseTest.hpp"
#include <filesystem>
#include <gtest/gtest.h>
#include <iostream>
#include <set>
#include <thread>

using namespace hive;

class GameTest : public BaseTest
{
  public:
    GameTest() : BaseTest() {}

    bool checkCounters(char tileType, int expectedCount)
    {
        bool result = applyAction(makePlaceAction({.x = 0, .y = 0}, tileType));
        for (int i = 1; i <= expectedCount; i++)
        {
            result = applyAction(makePlaceAction({.x = i, .y = 0}, tileType));
            if (!result)
            {
                break;
            }
            result = applyAction(makePlaceAction({.x = -i, .y = 0}, tileType));
        }
        return result;
    }
};

namespace
{
std::vector<Action> blackWins = {
    makePlaceAction({0, 0}, 'Q'),   makePlaceAction({1, 0}, 'Q'),   makePlaceAction({-1, 0}, 'A'),
    makePlaceAction({1, 1}, 'A'),   makePlaceAction({0, -1}, 'A'),  makePlaceAction({2, 0}, 'A'),
    makePlaceAction({-1, 1}, 'G'),  makeMoveAction({1, 1}, {0, 1}), makePlaceAction({-1, -1}, 'G'),
    makeMoveAction({2, 0}, {1, -1})

};

std::vector<Action> whiteWins = {makePlaceAction({0, 0}, 'Q'),    makePlaceAction({1, 0}, 'Q'),
                                 makePlaceAction({0, -1}, 'A'),   makePlaceAction({1, 1}, 'A'),
                                 makePlaceAction({-1, 0}, 'A'),   makePlaceAction({2, 0}, 'A'),
                                 makeMoveAction({0, -1}, {0, 1}), makePlaceAction({2, -1}, 'B'),
                                 makeMoveAction({-1, 0}, {1, -1})

};

std::vector<Action> draw = {makePlaceAction({0, 0}, 'Q'),
                            makePlaceAction({1, 0}, 'Q'),
                            makePlaceAction({-1, 1}, 'A'),
                            makePlaceAction({1, 1}, 'A'),
                            makePlaceAction({-1, 0}, 'A'),
                            makePlaceAction({2, 0}, 'A'),
                            makePlaceAction({0, -1}, 'A'),
                            makePlaceAction({2, -1}, 'A'),
                            makePlaceAction({-1, -1}, 'G'),
                            makePlaceAction({2, 1}, 'G'),
                            makeMoveAction({-1, -1}, {1, -1}),
                            makeMoveAction({2, 1}, {0, 1})

};
} // namespace

TEST_F(GameTest, PlaceNewTile)
{
    applyAction(makePlaceAction({.x = 0, .y = 0}, 'A'));

    const auto &tile = board.getTile({.x = 0, .y = 0});
    ASSERT_EQ(tile.type, 'A');
    ASSERT_EQ(tile.color, 'W');
    Position position = {.x = 0, .y = 0};
    ASSERT_EQ(tile.position, position);
}

TEST_F(GameTest, MoveExistingTile)
{
    ASSERT_EQ(getCurrentTurn(), 'W');
    ASSERT_TRUE(applyAction(makePlaceAction({0, 0}, 'Q')));
    ASSERT_EQ(getCurrentTurn(), 'B');

    ASSERT_FALSE(applyAction(makeMoveAction({0, 0}, {0, 1})));
    ASSERT_TRUE(applyAction(makePlaceAction({1, 0}, 'Q')));
    ASSERT_EQ(getCurrentTurn(), 'W');
    ASSERT_TRUE(applyAction(makeMoveAction({0, 0}, {0, 1})));

    const auto &tile = board.getTile({0, 1});
    ASSERT_EQ(tile.type, 'Q');
}

TEST_F(GameTest, CountersCheck)
{
    ASSERT_FALSE(checkCounters('A', 3));
    ASSERT_FALSE(checkCounters('G', 3));
    ASSERT_FALSE(checkCounters('S', 2));
    ASSERT_FALSE(checkCounters('B', 2));
    ASSERT_FALSE(checkCounters('Q', 1));
}

TEST_F(GameTest, BlackWins)
{
    playGame(blackWins);
    ASSERT_EQ(getGameStatus(), "BLACK_WINS");
}

TEST_F(GameTest, WhiteWins)
{
    playGame(whiteWins);

    ASSERT_EQ(getGameStatus(), "WHITE_WINS");
}

TEST_F(GameTest, Draw)
{
    playGame(draw);

    ASSERT_EQ(getGameStatus(), "DRAW");
}

TEST_F(GameTest, RejectedActionDoesNotChangeHistory)
{
    ASSERT_TRUE(applyAction(makePlaceAction({0, 0}, 'Q')));
    const std::string lastValidAction = getLastAction();

    ASSERT_FALSE(applyAction(makeMoveAction({0, 0}, {0, 1})));
    EXPECT_EQ(getLastAction(), lastValidAction);
    EXPECT_EQ(getPlyCount(), 1U);
}

TEST_F(GameTest, RevertingPlacedQueenRestoresQueenState)
{
    ASSERT_TRUE(applyAction(makePlaceAction({0, 0}, 'Q')));
    ASSERT_EQ(board.whiteQueen, (Position{0, 0}));

    revertAction();

    EXPECT_EQ(board.whiteQueen, invalidPosition);
    EXPECT_EQ(board.blackQueen, invalidPosition);
    EXPECT_FALSE(players['W'].queenPlaced);
    EXPECT_EQ(getCurrentTurn(), 'W');
    EXPECT_EQ(getPlyCount(), 0U);
}

TEST(GameMoveLimitTest, DrawsAtConfiguredPlyLimitAndCanBeReverted)
{
    Game game(2);

    ASSERT_TRUE(game.applyAction(makePlaceAction({0, 0}, 'Q')));
    EXPECT_FALSE(game.isGameOver());
    ASSERT_TRUE(game.applyAction(makePlaceAction({1, 0}, 'Q')));

    EXPECT_TRUE(game.isGameOver());
    EXPECT_EQ(game.getGameStatus(), "DRAW");
    EXPECT_EQ(game.getPlyCount(), 2U);

    game.revertAction();
    EXPECT_FALSE(game.isGameOver());
    EXPECT_EQ(game.getGameStatus(), "PLAYING");
    EXPECT_EQ(game.getPlyCount(), 1U);
}

TEST(GameMoveLimitTest, RejectsZeroLimit)
{
    EXPECT_THROW(Game(0), std::invalid_argument);
}
