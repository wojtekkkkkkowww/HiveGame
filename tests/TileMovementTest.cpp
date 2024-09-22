#include <set>
#include <gtest/gtest.h>
#include <iostream>
#include "Game.hpp"

class TileMovementTest : public ::testing::Test
{
protected:
    hive::Game game;
};

TEST_F(TileMovementTest, QueenBeeAvailableMovesAtStart)
{
    hive::Tile queenBee(hive::TileType::QUEEN, hive::Color::WHITE);
    game.board.addTile({0, 0}, queenBee);
    auto availableMoves = game.board.getAvailableMoves(queenBee);

    std::set<std::pair<int, int>> expectedMoves = {
        {1, -1}, {1, 0}, {0, 1}, {-1, 1}, {-1, 0}, {0, -1}};
    ASSERT_EQ(availableMoves.size(), 6);
    ASSERT_EQ(availableMoves, expectedMoves);
}

TEST_F(TileMovementTest, PlaceNewTile)
{

    game.placeTile({0, 0}, hive::TileType::ANT);

    auto tileList = game.board.boardTiles[{0, 0}];
    auto tile = tileList.front();
    ASSERT_EQ(tile.type, hive::TileType::ANT);
    ASSERT_EQ(tile.color, hive::Color::WHITE);
    ASSERT_EQ(tile.position, std::make_pair(0, 0));
}

TEST_F(TileMovementTest, MoveExistingTile)
{
    ASSERT_EQ(game.getCurrentTurn(), hive::Color::WHITE);
    game.placeTile({0, 0}, hive::TileType::QUEEN);
    ASSERT_EQ(game.getCurrentTurn(), hive::Color::BLACK);

    ASSERT_THROW(game.moveTile({0, 0}, {0, 1}), std::invalid_argument);
    game.placeTile({1, 0}, hive::TileType::QUEEN);
    ASSERT_EQ(game.getCurrentTurn(), hive::Color::WHITE);
    game.moveTile({0, 0}, {0, 1});

    auto tileList = game.board.boardTiles[{1, 0}];

    auto tile = tileList.front();
    ASSERT_EQ(tile.type, hive::TileType::QUEEN);
}

TEST_F(TileMovementTest, CountersCheck)
{
    game.placeTile({0, 0}, hive::TileType::ANT);
    game.placeTile({1, 0}, hive::TileType::ANT);
    game.placeTile({2, 0}, hive::TileType::ANT);
    game.placeTile({3, 0}, hive::TileType::ANT);
    game.placeTile({4, 0}, hive::TileType::ANT);
    game.placeTile({5, 0}, hive::TileType::ANT);
    ASSERT_THROW(game.placeTile({6, 0}, hive::TileType::ANT), std::invalid_argument);
}
