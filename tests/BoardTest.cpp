#include <set>
#include <gtest/gtest.h>
#include "Game.hpp"
#include <iostream>

class BoardTest : public ::testing::Test
{
protected:
    hive::Board board; // to jest bez sensu klasa BoarTest powinan dziedziczyć po klasie Board 
};

TEST_F(BoardTest, GetNeighboursAtOrigin)
{
    std::set<hive::Position> expectedNeighbours = {
        {1, -1}, {1, 0}, {0, 1}, {-1, 1}, {-1, 0}, {0, -1}};
    ASSERT_EQ(hive::Board::getNeighbours({0, 0}), expectedNeighbours);
}

TEST_F(BoardTest, GetNeighboursAtPosition)
{
    std::set<hive::Position> expectedNeighbours = {
        {3, -1}, {3, 0}, {2, 1}, {1, 1}, {1, 0}, {2, -1}};
    ASSERT_EQ(hive::Board::getNeighbours({2, 0}), expectedNeighbours);
}

TEST_F(BoardTest, GetTileAtEmptyPosition)
{
    EXPECT_THROW(board.getTile({0, 0}), std::invalid_argument);
    EXPECT_THROW(board.getTile({1, 0}), std::invalid_argument);
}

TEST_F(BoardTest, GetTileAtPosition)
{
    hive::Tile tile('Q',"WHITE");
    board.addTile({0, 0}, tile);
    auto retrievedTile = board.getTile({0, 0});
    
    ASSERT_TRUE(retrievedTile.type == tile.type && retrievedTile.color == tile.color);
}


TEST_F(BoardTest, RemoveTile)
{
    hive::Tile tile('Q',"WHITE");
    board.addTile({0, 0}, tile);
    board.removeTile({0, 0});
    EXPECT_THROW(board.getTile({0, 0}), std::invalid_argument);
}