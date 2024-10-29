#include <set>
#include <gtest/gtest.h>
#include <iostream>
#include <filesystem>
#include <thread>
#include "Game.hpp"
#include "BoardDrawable.hpp"
#include "BaseTest.hpp"

using namespace hive;

class GameTest : public BaseTest
{
public:
    GameTest() : BaseTest() {
        //saveState = true;
    }

    bool checkCounters(char tileType, int expectedCount)
    {
        bool result = applyAction(PlaceAction({0, 0}, tileType));
        for (int i = 1; i <= expectedCount; i++)
        {
            result = applyAction(PlaceAction({i, 0}, tileType));
            if (!result)
            {
                break;
            }
            result = applyAction(PlaceAction({-i, 0}, tileType));
        }
        return result;
    }
};

namespace
{
    std::vector<Action> blackWins = {
        PlaceAction({0, 0}, 'Q'),
        PlaceAction({1, 0}, 'Q'),
        PlaceAction({-1, 0}, 'A'),
        PlaceAction({1, 1}, 'A'),
        PlaceAction({0, -1}, 'A'),
        PlaceAction({2, 0}, 'A'),
        PlaceAction({-1, 1}, 'G'),
        MoveAction({1, 1}, {0, 1}),
        PlaceAction({-1, -1}, 'G'),
        MoveAction({2, 0}, {1, -1})

    };

    std::vector<Action> whiteWins = {
        PlaceAction({0, 0}, 'Q'),
        PlaceAction({1, 0}, 'Q'),
        PlaceAction({0, -1}, 'A'),
        PlaceAction({1, 1}, 'A'),
        PlaceAction({-1, 0}, 'A'),
        PlaceAction({2, 0}, 'A'),
        MoveAction({0, -1}, {0, 1}),
        PlaceAction({2, -1}, 'B'),
        MoveAction({-1, 0}, {1, -1})
    
    };

    std::vector<Action> draw = {
        PlaceAction({0, 0}, 'Q'),
        PlaceAction({1, 0}, 'Q'),
        PlaceAction({-1, 1}, 'A'),
        PlaceAction({1, 1}, 'A'),
        PlaceAction({-1, 0}, 'A'),
        PlaceAction({2, 0}, 'A'),
        PlaceAction({0, -1}, 'A'),
        PlaceAction({2, -1}, 'A'),
        PlaceAction({-1, -1}, 'G'),
        PlaceAction({2, 1}, 'G'),
        MoveAction({-1, -1}, {1, -1}),
        MoveAction({2, 1}, {0, 1})
    
    };
}

TEST_F(GameTest, PlaceNewTile)
{
    PlaceAction action{{0, 0}, 'A'};
    applyAction(action);

    const auto &tile = board.getTile({0, 0});
    ASSERT_EQ(tile.type, 'A');
    ASSERT_EQ(tile.color, 'W');
    Position position = {0, 0};
    ASSERT_EQ(tile.position, position);
}

TEST_F(GameTest, MoveExistingTile)
{
    ASSERT_EQ(getCurrentTurn(), 'W');
    ASSERT_TRUE(applyAction(PlaceAction({0, 0}, 'Q')));
    ASSERT_EQ(getCurrentTurn(), 'B');

    ASSERT_FALSE(applyAction(MoveAction{{0, 0}, {0, 1}}));
    ASSERT_TRUE(applyAction(PlaceAction({1, 0}, 'Q')));
    ASSERT_EQ(getCurrentTurn(), 'W');
    ASSERT_TRUE(applyAction(MoveAction({0, 0}, {0, 1})));

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