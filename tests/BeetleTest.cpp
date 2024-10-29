#include <gtest/gtest.h>
#include "BaseTest.hpp"

using namespace hive;

class BeetleTest : public BaseTest
{
public:
    BeetleTest() : BaseTest() {
    //    saveState = true;
    }
};

TEST_F(BeetleTest, BeetleJump)
{
    ASSERT_TRUE(applyAction(PlaceAction({0, 0}, 'Q')));
    ASSERT_TRUE(applyAction(PlaceAction({0, -1}, 'Q')));
    Position beetlePosition = {0, 1};
    ASSERT_TRUE(applyAction(PlaceAction(beetlePosition, 'B')));

    Position beetlePosition2 = {0, -2};
    ASSERT_TRUE(applyAction(PlaceAction(beetlePosition2, 'B')));        

    std::set<Position> expectedMoves = {{0, 0}, {-1, 1}, {1, 0}};

    checkMoves(beetlePosition, expectedMoves);
    ASSERT_TRUE(applyAction(MoveAction(beetlePosition, {0, 0})));

    expectedMoves = {{-1, -1}, {1, -2}, {0, -1}};
    checkMoves(beetlePosition2, expectedMoves);

    ASSERT_TRUE(applyAction(MoveAction(beetlePosition2, {0, -1})));
    ASSERT_TRUE(applyAction(MoveAction({0, 0}, {0, -1})));

    expectedMoves = {};
    checkMoves({0, -1}, expectedMoves);
    ASSERT_TRUE(applyAction(WaitAction()));
    ASSERT_TRUE(applyAction(MoveAction({0, -1}, {0, 0})));
    ASSERT_TRUE(applyAction(PlaceAction({-1, -1}, 'B')));
    ASSERT_TRUE(applyAction(PlaceAction({-1, 1}, 'B')));
    ASSERT_TRUE(applyAction(PlaceAction({0, -2}, 'A')));    
    ASSERT_TRUE(applyAction(MoveAction({0, 0}, {0, -1})));
    ASSERT_TRUE(applyAction(MoveAction({-1, -1}, {0, -1})));
}

TEST_F(BeetleTest, freedomToMove)
{
    std::map<Position,std::deque<hive::Tile>> tiles = 
    {   {{0,0},{Tile("WQ")}},
        {{-1,1},{Tile("WG1"),Tile("BB1")}},
        {{0,-1},{Tile("BQ"),Tile("WB1")}},
        {{-1,0},{Tile("BB2")}}
    };

    
    startGameFromState(tiles,'B');
    saveBoardState();
    
    auto actions = getAvailableActions();
    for(auto action : actions)
    {
        std::cerr << action << std::endl;
    }

    auto wrongAction = MoveAction({-1,0},{0,0});
    ASSERT_TRUE(actions.find(wrongAction) == actions.end());


    board.resetBoard();
    std::map<Position,std::deque<hive::Tile>> tiles2 = 
    {   {{0,0},{Tile("WQ"),Tile("WB1")}},
        {{-1,1},{Tile("WG1"),Tile("BB1")}},
        {{0,-1},{Tile("BQ"),Tile("WB2")}},
        {{-1,0},{Tile("BB2")}}
    };

    startGameFromState(tiles2,'B');
    saveBoardState();


    actions = getAvailableActions();
    auto correctAction = MoveAction({-1,0},{0,0});
    ASSERT_TRUE(actions.find(correctAction) != actions.end());


}