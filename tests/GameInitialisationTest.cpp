#include <vector>
#include <memory>
#include <gtest/gtest.h>
#include "HiveGameEngine.h"

class GameInitializationTest : public ::testing::Test {
protected:
    hge::Game game;
};


TEST_F(GameInitializationTest, BoardInit) {
    ASSERT_TRUE(game.board->pieces.empty());
    ASSERT_EQ(game.board->emptyTiles.size(), 1);
    
    auto type = game.board->emptyTiles[{0, 0}].type;
    ASSERT_EQ(type, hge::TileType::EMPTY);
}

TEST_F(GameInitializationTest, GameReset) {
    game.reset(); 
    ASSERT_TRUE(game.board->pieces.empty());
    ASSERT_EQ(game.board->emptyTiles.size(), 1);
    
    auto type = game.board->emptyTiles[{0, 0}].type;
    ASSERT_EQ(type, hge::TileType::EMPTY);
    ASSERT_EQ(game.player1.pieces.size(), 11);
    ASSERT_EQ(game.player2.pieces.size(), 11);
}