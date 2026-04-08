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
    GameTest() : BaseTest()
    {}

    bool checkCounters(char tileType, int expectedCount)
    {
        bool result = applyAction(Action::makePlaceAction({0, 0}, tileType));
        for (int i = 1; i <= expectedCount; i++)
        {
            result = applyAction(Action::makePlaceAction({i, 0}, tileType));
            if (!result)
            {
                break;
            }
            result = applyAction(Action::makePlaceAction({-i, 0}, tileType));
        }
        return result;
    }
};

namespace
{
std::vector<Action> blackWins = {
    Action::makePlaceAction({0, 0}, 'Q'),   Action::makePlaceAction({1, 0}, 'Q'),   Action::makePlaceAction({-1, 0}, 'A'),
    Action::makePlaceAction({1, 1}, 'A'),   Action::makePlaceAction({0, -1}, 'A'),  Action::makePlaceAction({2, 0}, 'A'),
    Action::makePlaceAction({-1, 1}, 'G'),  Action::makeMoveAction({1, 1}, {0, 1}), Action::makePlaceAction({-1, -1}, 'G'),
    Action::makeMoveAction({2, 0}, {1, -1})

};

std::vector<Action> whiteWins = {Action::makePlaceAction({0, 0}, 'Q'),    Action::makePlaceAction({1, 0}, 'Q'),
                                 Action::makePlaceAction({0, -1}, 'A'),   Action::makePlaceAction({1, 1}, 'A'),
                                 Action::makePlaceAction({-1, 0}, 'A'),   Action::makePlaceAction({2, 0}, 'A'),
                                 Action::makeMoveAction({0, -1}, {0, 1}), Action::makePlaceAction({2, -1}, 'B'),
                                 Action::makeMoveAction({-1, 0}, {1, -1})

};

std::vector<Action> draw = {Action::makePlaceAction({0, 0}, 'Q'),
                            Action::makePlaceAction({1, 0}, 'Q'),
                            Action::makePlaceAction({-1, 1}, 'A'),
                            Action::makePlaceAction({1, 1}, 'A'),
                            Action::makePlaceAction({-1, 0}, 'A'),
                            Action::makePlaceAction({2, 0}, 'A'),
                            Action::makePlaceAction({0, -1}, 'A'),
                            Action::makePlaceAction({2, -1}, 'A'),
                            Action::makePlaceAction({-1, -1}, 'G'),
                            Action::makePlaceAction({2, 1}, 'G'),
                            Action::makeMoveAction({-1, -1}, {1, -1}),
                            Action::makeMoveAction({2, 1}, {0, 1})

};
} // namespace

TEST_F(GameTest, PlaceNewTile)
{
    applyAction(Action::makePlaceAction({.x=0, .y=0}, 'A'));

    const auto &tile = board.getTile({.x=0, .y=0});
    ASSERT_EQ(tile.type, 'A');
    ASSERT_EQ(tile.color, 'W');
    Position position = {.x=0, .y=0};
    ASSERT_EQ(tile.position, position);
}

TEST_F(GameTest, MoveExistingTile)
{
    ASSERT_EQ(getCurrentTurn(), 'W');
    ASSERT_TRUE(applyAction(Action::makePlaceAction({0, 0}, 'Q')));
    ASSERT_EQ(getCurrentTurn(), 'B');

    ASSERT_FALSE(applyAction(Action::makeMoveAction({0, 0}, {0, 1})));
    ASSERT_TRUE(applyAction(Action::makePlaceAction({1, 0}, 'Q')));
    ASSERT_EQ(getCurrentTurn(), 'W');
    ASSERT_TRUE(applyAction(Action::makeMoveAction({0, 0}, {0, 1})));

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
