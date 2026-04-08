#include "BaseTest.hpp"
#include <gtest/gtest.h>

using namespace hive;

class GrasshopperTest : public BaseTest
{
  public:
    GrasshopperTest() : BaseTest()
    {
        // saveState = true;
    }
};

TEST_F(GrasshopperTest, GrasshopperMove)
{
    ASSERT_TRUE(applyAction(Action::makePlaceAction({0, 0}, 'Q')));
    ASSERT_TRUE(applyAction(Action::makePlaceAction({0, -1}, 'Q')));
    Position firstGrasshopperPosition = {.x=0, .y=1};
    ASSERT_TRUE(applyAction(Action::makePlaceAction(firstGrasshopperPosition, 'G')));
    Position secondGrasshopperPosition = {.x=0, .y=-2};
    ASSERT_TRUE(applyAction(Action::makePlaceAction(secondGrasshopperPosition, 'G')));

    std::set<Position> expectedMoves = {{0, -3}};
    checkMoves(firstGrasshopperPosition, expectedMoves);

    ASSERT_TRUE(applyAction(Action::makeMoveAction(firstGrasshopperPosition, {0, -3})));
}