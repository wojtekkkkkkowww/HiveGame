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
     
    ASSERT_TRUE(game.board.boardTiles.empty());
    ASSERT_EQ(game.board.emptyTiles.size(), 1);

    auto position = *game.board.emptyTiles.begin();
    ASSERT_EQ(position,hive::Position({0,0}));
   }

/*
to ma sprwadzać reset po zrobieniu kilku ruchów
*/
TEST_F(GameInitializationTest, GameReset)
{
    game.startNewGame();
}