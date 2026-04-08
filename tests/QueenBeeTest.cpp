#include "BaseTest.hpp"
#include <gtest/gtest.h>

using namespace hive;

class QueenBeeTest : public BaseTest
{
  public:
    QueenBeeTest() : BaseTest(){}
};

TEST_F(QueenBeeTest, QueenBeeAvailableMovesAtStart)
{
    Tile queenBee('Q', 'W');
    board.addTile({0, 0}, queenBee);
    queenBee = board.getTile({0, 0});
    std::set<Position> availableMoves;
    board.getAvailableMoves('Q', {0, 0}, availableMoves);

    std::set<Position> expectedMoves = {{1, -1}, {1, 0}, {0, 1}, {-1, 1}, {-1, 0}, {0, -1}};

    ASSERT_EQ(availableMoves.size(), 6);
    ASSERT_EQ(availableMoves, expectedMoves);
}

TEST_F(QueenBeeTest, CantMoveWithoutQueenPlaced)
{
    std::map<Position, std::deque<hive::Tile>> tiles = {{{0, 0}, {Tile("WA1")}},
                                                        {{0, 1}, {Tile("BA1")}}};

    startGameFromState(tiles, 'W');
    auto wrongAction = Action::makeMoveAction({0, 0}, {-1, 1});
    ASSERT_FALSE(applyAction(wrongAction));
}