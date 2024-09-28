#include <set>
#include <gtest/gtest.h>
#include "Game.hpp"
#include <iostream>

class BoardTest : public ::testing::Test
{
protected:
    hive::Board board;
};

TEST_F(BoardTest, GetNeighboursAtOrigin)
{
    std::set<hive::Position> expectedNeighbours = {
        {1, -1}, {1, 0}, {0, 1}, {-1, 1}, {-1, 0}, {0, -1}};
    ASSERT_EQ(board.getNeighbours({0, 0}), expectedNeighbours);
}

TEST_F(BoardTest, GetNeighboursAtPosition)
{
    std::set<hive::Position> expectedNeighbours = {
        {3, -1}, {3, 0}, {2, 1}, {1, 1}, {1, 0}, {2, -1}};
    ASSERT_EQ(board.getNeighbours({2, 0}), expectedNeighbours);
}

TEST_F(BoardTest, GetTileAtEmptyPosition)
{
    EXPECT_THROW(board.getTile({0, 0}), std::invalid_argument);
    EXPECT_THROW(board.getTile({1, 0}), std::invalid_argument);
}

TEST_F(BoardTest, GetTileAtPosition)
{
    hive::Tile tile("QUEEN","WHITE");
    board.addTile({0, 0}, tile);
    auto retrievedTile = board.getTile({0, 0});
    
    ASSERT_TRUE(retrievedTile.type == tile.type && retrievedTile.color == tile.color);
}

TEST_F(BoardTest, AddEmptyTilesAroundBoard)
{
    hive::Tile tile("QUEEN","WHITE");
    board.addTile({0, 0}, tile);
    board.addEmptyTilesAroundBoard();
    auto emptyTiles = board.emptyTiles;
    EXPECT_TRUE(emptyTiles.find({1, 0}) != emptyTiles.end());
    EXPECT_TRUE(emptyTiles.find({-1, 0}) != emptyTiles.end());
    EXPECT_TRUE(emptyTiles.find({0, 1}) != emptyTiles.end());
    EXPECT_TRUE(emptyTiles.find({0, -1}) != emptyTiles.end());
}

TEST_F(BoardTest, RemoveTile)
{
    hive::Tile tile("QUEEN","WHITE");
    board.addTile({0, 0}, tile);
    board.removeTile({0, 0});
    EXPECT_THROW(board.getTile({0, 0}), std::invalid_argument);
}