#include <gtest/gtest.h>
#include "BaseTest.hpp"

using namespace hive;

class SpiderTest : public BaseTest
{
public:
    SpiderTest() : BaseTest() {
    //    saveState = true;
    }
};

TEST_F(SpiderTest, spiderMove)
{
    ASSERT_TRUE(applyAction(PlaceAction({0, 0}, 'Q')));
    ASSERT_TRUE(applyAction(PlaceAction({0, -1}, 'Q')));
    ASSERT_TRUE(applyAction(PlaceAction({0, 1}, 'S')));
    ASSERT_TRUE(applyAction(PlaceAction({0, -2}, 'S')));

    std::set<Position> expectedMoves = {{-1, -1}, {1, -2}};
    checkMoves({0, 1}, expectedMoves);
}

TEST_F(SpiderTest, freedomToMove)
{
    std::map <Position, std::deque<hive::Tile>> tiles =
    {   {{1,0},{Tile("WQ")}},
        {{1,-1},{Tile("WG1")}},
        {{0,1},{Tile("BQ")}},
        {{-1,1},{Tile("BB1")}},
        {{0,-1},{Tile("WB1")}},  
        {{-2,2},{Tile("BS1")}}
    };
    startGameFromState(tiles, 'B');

    auto wrongAction = MoveAction({-2, 2}, {0, 0});
    ASSERT_FALSE(applyAction(wrongAction));
}

