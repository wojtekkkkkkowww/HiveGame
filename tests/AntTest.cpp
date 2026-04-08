#include "BaseTest.hpp"
#include <gtest/gtest.h>

using namespace hive;

class AntTest : public BaseTest
{
  public:
    AntTest() : BaseTest() {}
};

TEST_F(AntTest, AntMove)
{
    ASSERT_TRUE(applyAction(Action::makePlaceAction({0, 0}, 'Q')));
    ASSERT_TRUE(applyAction(Action::makePlaceAction({0, -1}, 'Q')));
    Position antPosition = {0, 1};
    ASSERT_TRUE(applyAction(Action::makePlaceAction(antPosition, 'A')));
    ASSERT_TRUE(applyAction(Action::makeMoveAction({0, -1}, {-1, 0})));

    std::set<Position> expectedMoves = {{-1, 1}, {1, 0},  {1, -1}, {-2, 1},
                                        {-2, 0}, {0, -1}, {-1, -1}};
    checkMoves(antPosition, expectedMoves);
    ASSERT_TRUE(applyAction(Action::makeMoveAction(antPosition, {-2, 0})));
}

TEST_F(AntTest, FredomToMove)
{
    std::map<Position, std::deque<hive::Tile>> tiles = {
        {{-1, 1}, {Tile("WQ")}},  {{-1, 0}, {Tile("WG1")}}, {{-1, -1}, {Tile("WB1")}},
        {{2, -1}, {Tile("BB1")}}, {{1, -2}, {Tile("BB2")}}, {{0, -1}, {Tile("BA1")}},
        {{0, 1}, {Tile("BA2")}},  {{1, 0}, {Tile("BA3")}},  {{2, -2}, {Tile("WG2")}}};

    startGameFromState(tiles, 'B');
    Action wrongMove = Action::makeMoveAction({0, -1}, {0, -2});
    ASSERT_FALSE(applyAction(wrongMove));
}