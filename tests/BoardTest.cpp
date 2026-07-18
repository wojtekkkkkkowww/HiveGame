#include "Game.hpp"
#include <gtest/gtest.h>
#include <iostream>
#include <set>

class BoardTest : public ::testing::Test
{
  protected:
    hive::Board board;
};

TEST_F(BoardTest, GetNeighboursAtOrigin)
{
    std::set<hive::Position> expectedNeighbours = {{1, -1}, {1, 0},  {0, 1},
                                                   {-1, 1}, {-1, 0}, {0, -1}};
    const auto neighbours = hive::Board::getNeighbours({0, 0});
    ASSERT_EQ((std::set<hive::Position>(neighbours.begin(), neighbours.end())), expectedNeighbours);
}

TEST_F(BoardTest, GetNeighboursAtPosition)
{
    std::set<hive::Position> expectedNeighbours = {{3, -1}, {3, 0}, {2, 1},
                                                   {1, 1},  {1, 0}, {2, -1}};
    const auto neighbours = hive::Board::getNeighbours({2, 0});
    ASSERT_EQ((std::set<hive::Position>(neighbours.begin(), neighbours.end())), expectedNeighbours);
}

TEST_F(BoardTest, GetTileAtPosition)
{
    hive::Tile tile('Q', 'W');
    board.addTile({0, 0}, tile);
    const auto &retrievedTile = board.getTile({0, 0});

    ASSERT_TRUE(retrievedTile.type == tile.type && retrievedTile.color == tile.color);
}

TEST_F(BoardTest, UsesFixedCapacityAndReusesReleasedSlot)
{
    static_assert(hive::Board::maxPieceCount == 22);

    for (std::size_t index = 0; index < hive::Board::maxPieceCount; ++index)
    {
        hive::Tile tile("WA" + std::to_string(index));
        board.addTile({static_cast<int>(index), 0}, tile);
    }

    EXPECT_EQ(board.getPieceCount(), hive::Board::maxPieceCount);
    EXPECT_THROW(board.addTile({22, 0}, hive::Tile("overflow")), std::length_error);

    board.removeTile({0, 0});
    EXPECT_EQ(board.getPieceCount(), hive::Board::maxPieceCount - 1);
    EXPECT_NO_THROW(board.addTile({22, 0}, hive::Tile("WA22")));
    EXPECT_EQ(board.getPieceCount(), hive::Board::maxPieceCount);
}
