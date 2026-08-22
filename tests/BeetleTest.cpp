#include "BaseTest.hpp"
#include <algorithm>
#include <gtest/gtest.h>

using namespace hive;

class BeetleTest : public BaseTest
{
  public:
    BeetleTest() : BaseTest() {}
};

TEST_F(BeetleTest, BeetleJump)
{
    ASSERT_TRUE(applyAction(makePlaceAction({0, 0}, PieceType::Queen)));
    ASSERT_TRUE(applyAction(makePlaceAction({0, -1}, PieceType::Queen)));
    Position beetlePosition = {.x = 0, .y = 1};
    ASSERT_TRUE(applyAction(makePlaceAction(beetlePosition, PieceType::Beetle)));

    Position beetlePosition2 = {.x = 0, .y = -2};
    ASSERT_TRUE(applyAction(makePlaceAction(beetlePosition2, PieceType::Beetle)));

    std::set<Position> expectedMoves = {{0, 0}, {-1, 1}, {1, 0}};

    checkMoves(beetlePosition, expectedMoves);
    ASSERT_TRUE(applyAction(makeMoveAction(beetlePosition, {0, 0})));

    expectedMoves = {{-1, -1}, {1, -2}, {0, -1}};
    checkMoves(beetlePosition2, expectedMoves);

    ASSERT_TRUE(applyAction(makeMoveAction(beetlePosition2, {0, -1})));
    ASSERT_TRUE(applyAction(makeMoveAction({0, 0}, {0, -1})));

    expectedMoves = {};
    checkMoves({.x = 0, .y = -1}, expectedMoves);
    ASSERT_TRUE(applyAction(makeWaitAction()));
    ASSERT_TRUE(applyAction(makeMoveAction({0, -1}, {0, 0})));
    ASSERT_TRUE(applyAction(makePlaceAction({-1, -1}, PieceType::Beetle)));
    ASSERT_TRUE(applyAction(makePlaceAction({-1, 1}, PieceType::Beetle)));
    ASSERT_TRUE(applyAction(makePlaceAction({0, -2}, PieceType::Ant)));
    ASSERT_TRUE(applyAction(makeMoveAction({0, 0}, {0, -1})));
    ASSERT_TRUE(applyAction(makeMoveAction({-1, -1}, {0, -1})));
}

TEST_F(BeetleTest, freedomToMove)
{
    std::map<Position, std::deque<hive::Tile>> tiles = {{{0, 0}, {Tile("WQ")}},
                                                        {{-1, 1}, {Tile("WG1"), Tile("BB1")}},
                                                        {{0, -1}, {Tile("BQ"), Tile("WB1")}},
                                                        {{-1, 0}, {Tile("BB2")}}};

    startGameFromState(tiles, PlayerColor::Black);
    // saveBoardState();

    auto actions = availableActions;
    for (const auto &action : actions)
    {
        std::cerr << action << std::endl;
    }

    auto wrongAction = makeMoveAction({-1, 0}, {0, 0});
    ASSERT_TRUE(std::find(actions.begin(), actions.end(), wrongAction) == actions.end());

    board.resetBoard();
    std::map<Position, std::deque<hive::Tile>> tiles2 = {{{0, 0}, {Tile("WQ"), Tile("WB1")}},
                                                         {{-1, 1}, {Tile("WG1"), Tile("BB1")}},
                                                         {{0, -1}, {Tile("BQ"), Tile("WB2")}},
                                                         {{-1, 0}, {Tile("BB2")}}};

    startGameFromState(tiles2, PlayerColor::Black);
    // saveBoardState();

    actions = availableActions;
    auto correctAction = makeMoveAction({-1, 0}, {0, 0});
    ASSERT_TRUE(std::find(actions.begin(), actions.end(), correctAction) != actions.end());
}