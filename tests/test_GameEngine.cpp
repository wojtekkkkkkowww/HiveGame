#include <gtest/gtest.h>
#include "HiveGameEngine.h"

TEST(GameEngineTest, GameConstructor) {
    hge::Game game;
    SUCCEED();
}

TEST(GameEngineTest, StartGame) {
    hge::Game game;
    game.start();
    /*
    check avaliable pieces for both players
    */
}

TEST(GameEngineTest, TurnAfterStart) {
    hge::Game game;
    game.start();
    ASSERT_EQ(game.getTurn(), hge::Turn::PLAYER1);
}