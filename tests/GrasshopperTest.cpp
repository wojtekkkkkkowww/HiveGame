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
    ASSERT_TRUE(applyAction(makePlaceAction({0, 0}, PieceType::Queen)));
    ASSERT_TRUE(applyAction(makePlaceAction({0, -1}, PieceType::Queen)));
    Position firstGrasshopperPosition = {.x = 0, .y = 1};
    ASSERT_TRUE(applyAction(makePlaceAction(firstGrasshopperPosition, PieceType::Grasshopper)));
    Position secondGrasshopperPosition = {.x = 0, .y = -2};
    ASSERT_TRUE(applyAction(makePlaceAction(secondGrasshopperPosition, PieceType::Grasshopper)));

    std::set<Position> expectedMoves = {{0, -3}};
    checkMoves(firstGrasshopperPosition, expectedMoves);

    ASSERT_TRUE(applyAction(makeMoveAction(firstGrasshopperPosition, {0, -3})));
}