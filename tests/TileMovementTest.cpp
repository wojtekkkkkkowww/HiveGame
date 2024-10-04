#include <set>
#include <gtest/gtest.h>
#include <iostream>
#include "Game.hpp"
#include "BoardDrawable.hpp" // Include the header for //saveBoardAsPng

using namespace hive;

class TileMovementTest : public ::testing::Test
{
protected:
    Game game; // to powinno dziedziczyć po game a nie mieć jaki obiekt kurwa !!!
};
void checkMoves(Position position, Game &game, const std::set<Position> &expectedMoves)
{
    try
    {
        auto actions = game.getAvailableActions();
        std::set<Position> actualMoves{};

        for (const auto &action : actions)
        {
            if (action.type == "MOVE" && action.position == position)
            {
                actualMoves.insert(action.newPosition);
            }
        }

        ASSERT_EQ(actualMoves, expectedMoves);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}
void saveBoardAsPng(const Board &board, const std::string &filename, float hexSize = 32.0f)
{
    sf::RenderTexture renderTexture;
    renderTexture.create(800, 600);

    BoardDrawable boardDrawable(board, hexSize);

    renderTexture.clear(sf::Color::White);
    renderTexture.draw(boardDrawable);
    renderTexture.display();

    if (renderTexture.getTexture().copyToImage().saveToFile(filename))
    {
        std::cout << "Saved board state to " << filename << std::endl;
    }
}

TEST_F(TileMovementTest, QueenBeeAvailableMovesAtStart)
{
    Tile queenBee("QUEEN", "WHITE");
    game.board.addTile({0, 0}, queenBee);
    auto availableMoves = game.board.getAvailableMoves(queenBee);

    std::set<Position> expectedMoves = {
        {1, -1}, {1, 0}, {0, 1}, {-1, 1}, {-1, 0}, {0, -1}};

    ASSERT_EQ(availableMoves.size(), 6);

    ASSERT_EQ(availableMoves, expectedMoves);

    // Save the board state as a PNG file
    saveBoardAsPng(game.board, "QueenBeeAvailableMovesAtStart.png");
}

TEST_F(TileMovementTest, PlaceNewTile)
{
    // Action action{"PLACE", {0, 0}, "ANT"};
    PlaceAction action{{0, 0}, "ANT"};
    game.applyAction(action);

    auto tile = game.board.getTile({0, 0});
    ASSERT_EQ(tile.type, "ANT");
    ASSERT_EQ(tile.color, "WHITE");
    Position position = {0, 0};
    ASSERT_EQ(tile.position, position);

    // Save the board state as a PNG file
    saveBoardAsPng(game.board, "PlaceNewTile.png");
}

TEST_F(TileMovementTest, MoveExistingTile)
{
    ASSERT_EQ(game.getCurrentTurn(), "WHITE");
    game.applyAction(PlaceAction({0, 0}, "QUEEN"));
    ASSERT_EQ(game.getCurrentTurn(), "BLACK");

    ASSERT_THROW(game.applyAction(MoveAction{{0, 0}, {0, 1}}), std::invalid_argument);
    game.applyAction(PlaceAction({1, 0}, "QUEEN"));
    ASSERT_EQ(game.getCurrentTurn(), "WHITE");
    game.applyAction(MoveAction({0, 0}, {0, 1}));

    auto tile = game.board.getTile({0, 1});
    ASSERT_EQ(tile.type, "QUEEN");

    // Save the board state as a PNG file
    saveBoardAsPng(game.board, "MoveExistingTile.png");
}

void checkCounters(Game &game, const std::string &tileType, int expectedCount)
{
    game.applyAction(PlaceAction({0, 0}, tileType));
    for (int i = 1; i <= expectedCount; i++)
    {
        game.applyAction(PlaceAction({i, 0}, tileType));
        game.applyAction(PlaceAction({-i, 0}, tileType));
    }
}

TEST_F(TileMovementTest, CountersCheck)
{
    ASSERT_THROW(checkCounters(game, "ANT", 3), std::invalid_argument);
    ASSERT_THROW(checkCounters(game, "GRASSHOPPER", 3), std::invalid_argument);
    ASSERT_THROW(checkCounters(game, "SPIDER", 2), std::invalid_argument);
    ASSERT_THROW(checkCounters(game, "BEETLE", 2), std::invalid_argument);
    ASSERT_THROW(checkCounters(game, "QUEEN", 1), std::invalid_argument);
}

TEST_F(TileMovementTest, BlackWins)
{
    game.applyAction(PlaceAction({0, 0}, "QUEEN"));
    game.applyAction(PlaceAction({1, 0}, "QUEEN"));
    game.applyAction(PlaceAction({0, 1}, "ANT"));
    game.applyAction(PlaceAction({1, 1}, "ANT"));
    game.applyAction(PlaceAction({0, -1}, "ANT"));
    game.applyAction(PlaceAction({1, -1}, "ANT"));
    game.applyAction(PlaceAction({-1, 0}, "GRASSHOPPER"));
    game.applyAction(MoveAction({1, 1}, {-1, 1}));

    saveBoardAsPng(game.board, "BlackWins.png");
    ASSERT_EQ(game.getGameStatus(), "BLACK_WINS");
}

TEST_F(TileMovementTest, WhiteWins)
{
    game.applyAction(PlaceAction({0, 0}, "QUEEN"));
    game.applyAction(PlaceAction({1, 0}, "QUEEN"));
    game.applyAction(PlaceAction({0, 1}, "ANT"));
    game.applyAction(PlaceAction({1, 1}, "ANT"));
    game.applyAction(PlaceAction({0, -1}, "ANT"));
    game.applyAction(PlaceAction({1, -1}, "ANT"));
    game.applyAction(PlaceAction({-1, 0}, "GRASSHOPPER"));
    game.applyAction(PlaceAction({2, -1}, "BEETLE"));
    game.applyAction(MoveAction({-1, 0}, {2, 0}));

    saveBoardAsPng(game.board, "WhiteWins.png");
    ASSERT_EQ(game.getGameStatus(), "WHITE_WINS");
}

TEST_F(TileMovementTest, Draw)
{
    game.applyAction(PlaceAction({0, 0}, "QUEEN"));
    game.applyAction(PlaceAction({1, 0}, "QUEEN"));
    game.applyAction(PlaceAction({0, 1}, "ANT"));
    game.applyAction(PlaceAction({1, 1}, "ANT"));
    game.applyAction(PlaceAction({-1, 1}, "ANT"));
    game.applyAction(PlaceAction({2, 0}, "GRASSHOPPER"));
    game.applyAction(PlaceAction({-1, 0}, "GRASSHOPPER"));
    game.applyAction(PlaceAction({2, -1}, "GRASSHOPPER"));
    game.applyAction(PlaceAction({0, -1}, "GRASSHOPPER"));
    game.applyAction(PlaceAction({1, -1}, "GRASSHOPPER"));

    saveBoardAsPng(game.board, "Draw.png");
    ASSERT_EQ(game.getGameStatus(), "DRAW");
}

TEST_F(TileMovementTest, GrasshopperMove)
{
    game.applyAction(PlaceAction({0, 0}, "QUEEN"));
    game.applyAction(PlaceAction({0, -1}, "QUEEN"));
    Position firstGrasshopperPosition = {0, 1};
    game.applyAction(PlaceAction(firstGrasshopperPosition, "GRASSHOPPER"));
    Position secondGrasshopperPosition = {0, -2};
    game.applyAction(PlaceAction(secondGrasshopperPosition, "GRASSHOPPER"));

    std::set<Position> expectedMoves = {{0, -3}};
    checkMoves(firstGrasshopperPosition, game, expectedMoves);

    game.applyAction(MoveAction(firstGrasshopperPosition, {0, -3}));

    // Save the board state as a PNG file
    saveBoardAsPng(game.board, "GrasshopperAvailableMoves.png");
}

TEST_F(TileMovementTest, BeetleJump)
{
    game.applyAction(PlaceAction({0, 0}, "QUEEN"));
    game.applyAction(PlaceAction({0, -1}, "QUEEN"));
    Position beetlePosition = {0, 1};
    game.applyAction(PlaceAction(beetlePosition, "BEETLE"));

    Position beetlePosition2 = {0, -2};
    game.applyAction(PlaceAction(beetlePosition2, "BEETLE"));

    std::set<Position> expectedMoves = {{0, 0}, {-1, 1}, {1, 0}};
    checkMoves(beetlePosition, game, expectedMoves);
    game.applyAction(MoveAction(beetlePosition, {0, 0}));

    expectedMoves = {{-1, -1}, {1, -2}, {0, -1}};
    checkMoves(beetlePosition2, game, expectedMoves);

    game.applyAction(MoveAction(beetlePosition2, {0, -1}));

    saveBoardAsPng(game.board, "BeetleJump.png");

    game.applyAction(MoveAction({0, 0}, {0, -1}));
    expectedMoves = {};
    checkMoves({0, -1}, game, expectedMoves);
}

TEST_F(TileMovementTest, AntMove)
{
    game.applyAction(PlaceAction({0, 0}, "QUEEN"));
    game.applyAction(PlaceAction({0, -1}, "QUEEN"));
    Position antPosition = {0, 1};
    game.applyAction(PlaceAction(antPosition, "ANT"));
    game.applyAction(MoveAction({0, -1}, {-1, 0}));
    saveBoardAsPng(game.board, "AntMove1.png");

    std::set<Position> expectedMoves = {{-1, 1}, {1, 0}, {1, -1}, {-2, 1}, {-2, 0}, {0, -1}, {-1, -1}};
    checkMoves(antPosition, game, expectedMoves);
    game.applyAction(MoveAction(antPosition, {-2, 0}));

    saveBoardAsPng(game.board, "AntMove2.png");
}
