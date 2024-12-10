#include <vector>
#include <memory>
#include <gtest/gtest.h>
#include "Game.hpp"
    
class GameInitializationTest : public ::testing::Test
{
protected:
    hive::Game game;
};

TEST_F(GameInitializationTest, BoardInit)
{
     
    ASSERT_EQ(game.board.getTileCount(), 0);
    
    ASSERT_TRUE(game.board.emptyTiles.find({0,0}) != game.board.emptyTiles.end()); 
    ASSERT_TRUE(game.board.emptyTiles.size() == 1);
}


TEST_F(GameInitializationTest, GameReset)
{
    game.startNewGame();
}