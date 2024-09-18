#include <set>
#include <gtest/gtest.h>
#include "HiveGameEngine.h"

class PieceMovementTest : public ::testing::Test
{
protected:
    hge::Game game;
};

TEST_F(PieceMovementTest, QueenBeeAvailableMovesAtStart)
{
    auto queenBee = std::make_shared<hge::QueenBee>(game.board);
    game.board->addPiece({0, 0}, queenBee);
    auto availableMoves = queenBee->getAvailableMoves();

    std::set<std::pair<int, int>> expectedMoves = {
        {1, -1}, {1, 0}, {0, 1}, {-1, 1}, {-1, 0}, {0, -1}};
    ASSERT_EQ(availableMoves.size(), 6);
    ASSERT_EQ(availableMoves, expectedMoves);
}

TEST_F(PieceMovementTest, PlaceNewTile)
{
    auto ant = std::make_shared<hge::Ant>(game.board);
    game.player1.placeTile({0, 0}, ant);

    auto pieceList = game.board->pieces[{0, 0}];
    auto piece = pieceList.front();
    ASSERT_EQ(piece, ant); // is this even correct? this compare the pointer, not the object!!??
    ASSERT_TRUE(ant->placed);
}

TEST_F(PieceMovementTest, MoveExistingTile)
{
    auto ant = std::make_shared<hge::Ant>(game.board);
    game.player1.placeTile({0, 0}, ant);
    game.player1.moveTile({0, 0}, {1, 0});

    auto pieceList = game.board->pieces[{1, 0}];
    auto piece = pieceList.front();

    ASSERT_EQ(piece, ant);
    ASSERT_EQ(game.board->pieces.find({0, 0}), game.board->pieces.end());
    ASSERT_TRUE(ant->placed);
}

TEST_F(PieceMovementTest, CannotPlaceAlreadyPlacedTile)
{
    auto ant = std::make_shared<hge::Ant>(game.board);
    game.player1.placeTile({0, 0}, ant);
    game.player1.placeTile({1, 0}, ant);
    // ushould throw an exception
}

TEST_F(PieceMovementTest, RemoveTile)
{
    auto ant = std::make_shared<hge::Ant>(game.board);
    game.player1.placeTile({0, 0}, ant);
    game.board->removeTile({0, 0});

    ASSERT_EQ(game.board->pieces.find({0, 0}), game.board->pieces.end());
    ASSERT_TRUE(game.board->isEmptyTile({0, 0}));
    ASSERT_FALSE(ant->placed);
}
