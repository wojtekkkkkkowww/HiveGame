#include "Game.hpp"
#include <gtest/gtest.h>
#include <memory>
#include <vector>

class GameInitializationTest : public ::testing::Test
{
  protected:
    hive::Game game;
};

TEST_F(GameInitializationTest, BoardInit)
{

    ASSERT_EQ(game.board.getTileCount(), 0);

    ASSERT_TRUE(game.board.emptyTiles.find({0, 0}) != game.board.emptyTiles.end());
    ASSERT_TRUE(game.board.emptyTiles.size() == 1);
}

TEST_F(GameInitializationTest, GameReset) { game.startNewGame(); }