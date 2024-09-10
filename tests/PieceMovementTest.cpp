#include <set>
#include <gtest/gtest.h>
#include "HiveGameEngine.h"

class PieceMovementTest : public ::testing::Test {
protected:
    hge::Game game;
};

TEST_F(PieceMovementTest, QueenBeeAvailableMovesAtStart) {
    auto queenBee = std::make_shared<hge::QueenBee>(game.board);
    game.board->addTile({0, 0}, queenBee);
    auto availableMoves = queenBee->getAvailableMoves();

    std::set<std::pair<int, int>> expectedMoves = {
        {1, -1}, {1, 0}, {0, 1}, {-1, 1}, {-1, 0}, {0, -1}
    };
    ASSERT_EQ(availableMoves.size(), 6);
    ASSERT_EQ(availableMoves, expectedMoves);
}

// Additional tests for other pieces and scenarios can be added here