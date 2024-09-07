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
    std::vector<hge::Tile> startPieces = {
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
    ASSERT_EQ(game.player1.tiles, startPieces);
    ASSERT_EQ(game.player2.tiles, startPieces);

}

TEST_F(GameEngineTest, BoardInit) {
    
    /*
    check if board has no pieces after game start
    */
    ASSERT_TRUE(game.board.tiles.empty());
    std::cout << game.board.emptyTiles.size() << std::endl;
    ASSERT_EQ(game.board.emptyTiles.size(), 1);
    auto tileType = game.board.emptyTiles[{0,0}].type;
    ASSERT_EQ(tileType, hge::TileType::EMPTY );
}



TEST_F(GameEngineTest, TurnAfterStart) {
    ASSERT_EQ(game.getTurn(), hge::Turn::PLAYER1);
}