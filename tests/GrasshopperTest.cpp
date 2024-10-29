#include <gtest/gtest.h>
#include "BaseTest.hpp"

using namespace hive;

class GrasshopperTest : public BaseTest
{
public:
    GrasshopperTest() : BaseTest() {
        //saveState = true;
    }
};

TEST_F(GrasshopperTest, GrasshopperMove)
{
    ASSERT_TRUE(applyAction(PlaceAction({0, 0}, 'Q')));
    ASSERT_TRUE(applyAction(PlaceAction({0, -1}, 'Q')));
    Position firstGrasshopperPosition = {0, 1};
    ASSERT_TRUE(applyAction(PlaceAction(firstGrasshopperPosition, 'G')));
    Position secondGrasshopperPosition = {0, -2};
    ASSERT_TRUE(applyAction(PlaceAction(secondGrasshopperPosition, 'G')));

    std::set<Position> expectedMoves = {{0, -3}};
    checkMoves(firstGrasshopperPosition, expectedMoves);

    ASSERT_TRUE(applyAction(MoveAction(firstGrasshopperPosition, {0, -3})));
}