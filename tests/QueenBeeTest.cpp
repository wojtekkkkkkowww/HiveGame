#include "BaseTest.hpp"
#include <gtest/gtest.h>

using namespace hive;

class QueenBeeTest : public BaseTest
{
  public:
    QueenBeeTest() : BaseTest() {}
};

TEST_F(QueenBeeTest, QueenBeeAvailableMovesAtStart)
{
    Tile queenBee('Q', 'W');
    board.addTile({.x = 0, .y = 0}, queenBee);
    queenBee = board.getTile({.x = 0, .y = 0});
    std::set<Position> availableMoves;
    board.getAvailableMoves('Q', {.x = 0, .y = 0}, availableMoves);

    std::set<Position> expectedMoves = {{1, -1}, {1, 0}, {0, 1}, {-1, 1}, {-1, 0}, {0, -1}};

    ASSERT_EQ(availableMoves.size(), 6);
    ASSERT_EQ(availableMoves, expectedMoves);
}

TEST_F(QueenBeeTest, CantMoveWithoutQueenPlaced)
{
    std::map<Position, std::deque<hive::Tile>> tiles = {{{.x = 0, .y = 0}, {Tile("WA1")}},
                                                        {{.x = 0, .y = 1}, {Tile("BA1")}}};

    startGameFromState(tiles, 'W');
    auto wrongAction = makeMoveAction({.x = 0, .y = 0}, {.x = -1, .y = 1});
    ASSERT_FALSE(applyAction(wrongAction));
}