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
    
    auto emptyTiles = game.board.getEmptyTiles();
    ASSERT_TRUE(emptyTiles.find({0,0}) != emptyTiles.end()); 
    ASSERT_TRUE(emptyTiles.size() == 1);
}

/*
to ma sprwadzać reset po zrobieniu kilku ruchów
*/
TEST_F(GameInitializationTest, GameReset)
{
    game.startNewGame();
}