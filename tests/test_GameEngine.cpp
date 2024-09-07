#include <vector>
#include <gtest/gtest.h>
#include "HiveGameEngine.h"


class GameEngineTest : public ::testing::Test {
protected:
    void SetUp() override {    }

    hge::Game game;
};

TEST_F(GameEngineTest, GameConstructor) {
    SUCCEED();
}

TEST_F(GameEngineTest, PiecesAtStart) {
    /*
    check available pieces for both players
    */
    std::vector<hge::Piece> startPieces = {
        hge::QueenBee(),
        hge::Spider(),
        hge::Spider(),
        hge::Beetle(),
        hge::Beetle(),
        hge::Beetle(),
        hge::GrassHopper(),
        hge::GrassHopper(),
        hge::GrassHopper(),
        hge::Ant(),
        hge::Ant()
    };
    /*
    check if both players have the same pieces
    */
    ASSERT_EQ(game.player1.pieces, startPieces);
    ASSERT_EQ(game.player2.pieces, startPieces);

}

TEST_F(GameEngineTest, BoardInit) {
    
    /*
    check if board has no pieces after game start
    */
    ASSERT_TRUE(game.board.tiles.empty());


}



TEST_F(GameEngineTest, TurnAfterStart) {
    ASSERT_EQ(game.getTurn(), hge::Turn::PLAYER1);
}