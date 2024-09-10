#include <set>
#include <gtest/gtest.h>
#include "HiveGameEngine.h"

class BoardTest : public ::testing::Test {
protected:
    hge::HiveBoard board;
};

TEST_F(BoardTest, GetNeighboursAtOrigin) {
    hge::EmptyTile tile;
    tile.setPosition({0, 0});
    std::set<std::pair<int, int>> expectedNeighbours = {
        {1, -1}, {1, 0}, {0, 1}, {-1, 1}, {-1, 0}, {0, -1}
    };
    ASSERT_EQ(tile.getNeighbours(), expectedNeighbours);
}

TEST_F(BoardTest, GetNeighboursAtPosition) {
    hge::EmptyTile tile;
    tile.setPosition({2, 0});
    std::set<std::pair<int, int>> expectedNeighbours = {
        {3, -1}, {3, 0}, {2, 1}, {1, 1}, {1, 0}, {2, -1}
    };
    ASSERT_EQ(tile.getNeighbours(), expectedNeighbours);
}