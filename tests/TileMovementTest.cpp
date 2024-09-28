#include <set>
#include <gtest/gtest.h>
#include <iostream>
#include "Game.hpp"
#include "BoardDrawable.hpp" // Include the header for saveBoardAsPng

class TileMovementTest : public ::testing::Test
{
protected:
    hive::Game game;
};


void saveBoardAsPng(const hive::Board &board, const std::string &filename, float hexSize = 32.0f)
{   
    sf::RenderTexture renderTexture;
    renderTexture.create(800, 600); // Adjust the size as needed
    BoardDrawable boardDrawable(board, hexSize);
    renderTexture.clear(sf::Color::White); // Background color
    renderTexture.draw(boardDrawable);
    renderTexture.display();
    if (renderTexture.getTexture().copyToImage().saveToFile(filename))
    {
        std::cout << "Saved board state to " << filename << std::endl;
    }
}

TEST_F(TileMovementTest, QueenBeeAvailableMovesAtStart)
{
    hive::Tile queenBee("QUEEN", "WHITE");
    game.board.addTile({0, 0}, queenBee);
    auto availableMoves = game.board.getAvailableMoves(queenBee);

    std::set<hive::Position> expectedMoves = {
        {1, -1}, {1, 0}, {0, 1}, {-1, 1}, {-1, 0}, {0, -1}};

    ASSERT_EQ(availableMoves.size(), 6);

    ASSERT_EQ(availableMoves, expectedMoves);

    // Save the board state as a PNG file
    saveBoardAsPng(game.board, "QueenBeeAvailableMovesAtStart.png");
}

TEST_F(TileMovementTest, PlaceNewTile)
{
    hive::Action action{"PLACE", hive::Position{0, 0}, "ANT"};
    game.applyAction(action);

    auto tile = game.board.getTile({0, 0});
    ASSERT_EQ(tile.type, "ANT");
    ASSERT_EQ(tile.color, "WHITE");
    hive::Position position = hive::Position{0, 0};
    ASSERT_EQ(tile.position, position);

    // Save the board state as a PNG file
    saveBoardAsPng(game.board, "PlaceNewTile.png");
}

TEST_F(TileMovementTest, MoveExistingTile)
{
    ASSERT_EQ(game.getCurrentTurn(), "WHITE");
    game.applyAction({"PLACE", hive::Position{0, 0}, "QUEEN"});
    ASSERT_EQ(game.getCurrentTurn(), "BLACK");

    ASSERT_THROW(game.applyAction({"MOVE", hive::Position{0, 0}, hive::Position{0, 1}}), std::invalid_argument);
    game.applyAction({"PLACE", hive::Position{1, 0}, "QUEEN"});
    ASSERT_EQ(game.getCurrentTurn(), "WHITE");
    game.applyAction({"MOVE", hive::Position{0, 0}, hive::Position{0, 1}});

    auto tile = game.board.getTile({0, 1});
    ASSERT_EQ(tile.type, "QUEEN");

    // Save the board state as a PNG file
    saveBoardAsPng(game.board, "MoveExistingTile.png");
}


TEST_F(TileMovementTest, CountersCheck)
{
    game.applyAction({"PLACE", hive::Position{0, 0}, "ANT"});
    game.applyAction({"PLACE", hive::Position{1, 0}, "ANT"});
    game.applyAction({"PLACE", hive::Position{-1, 0}, "ANT"});
    game.applyAction({"PLACE", hive::Position{2, 0}, "ANT"});
    game.applyAction({"PLACE", hive::Position{-2, 0}, "ANT"});
    game.applyAction({"PLACE", hive::Position{3, 0}, "ANT"});
    saveBoardAsPng(game.board, "CountersCheck.png");

    ASSERT_THROW(game.applyAction({"PLACE", hive::Position{-3, 0}, "ANT"}), std::invalid_argument);

}