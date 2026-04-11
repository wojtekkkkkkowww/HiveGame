#include "BaseTest.hpp"
#include <gtest/gtest.h>

using namespace hive;

class SpiderTest : public BaseTest
{
  public:
    SpiderTest() : BaseTest() {}
};

TEST_F(SpiderTest, spiderMove)
{
    ASSERT_TRUE(applyAction(makePlaceAction({0, 0}, 'Q')));
    ASSERT_TRUE(applyAction(makePlaceAction({0, -1}, 'Q')));
    ASSERT_TRUE(applyAction(makePlaceAction({0, 1}, 'S')));
    ASSERT_TRUE(applyAction(makePlaceAction({0, -2}, 'S')));

    std::set<Position> expectedMoves = {{-1, -1}, {1, -2}};
    checkMoves({.x = 0, .y = 1}, expectedMoves);
}

TEST_F(SpiderTest, freedomToMove)
{
    std::map<Position, std::deque<hive::Tile>> tiles = {
        {{.x = 1, .y = 0}, {Tile("WQ")}},   {{.x = 1, .y = -1}, {Tile("WG1")}},
        {{.x = 0, .y = 1}, {Tile("BQ")}},   {{.x = -1, .y = 1}, {Tile("BB1")}},
        {{.x = 0, .y = -1}, {Tile("WB1")}}, {{.x = -2, .y = 2}, {Tile("BS1")}}};
    startGameFromState(tiles, 'B');

    auto wrongAction = makeMoveAction({.x = -2, .y = 2}, {.x = 0, .y = 0});
    ASSERT_FALSE(applyAction(wrongAction));
}
