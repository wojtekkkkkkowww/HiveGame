#include <vector>
#include <memory>
#include <gtest/gtest.h>
#include "HiveGameEngine.h"

class GameInitializationTest : public ::testing::Test {
protected:
    hge::Game game;
};

TEST_F(GameInitializationTest, PiecesAtStart) {
    std::vector<std::shared_ptr<hge::Tile>> startPieces = {
        std::make_shared<hge::QueenBee>(),
        std::make_shared<hge::Spider>(),
        std::make_shared<hge::Spider>(),
        std::make_shared<hge::Beetle>(),
        std::make_shared<hge::Beetle>(),
        std::make_shared<hge::Beetle>(),
        std::make_shared<hge::GrassHopper>(),
        std::make_shared<hge::GrassHopper>(),
        std::make_shared<hge::GrassHopper>(),
        std::make_shared<hge::Ant>(),
        std::make_shared<hge::Ant>()
    };

    ASSERT_EQ(game.player1.tiles.size(), startPieces.size());
    ASSERT_EQ(game.player2.tiles.size(), startPieces.size());

    for (size_t i = 0; i < startPieces.size(); ++i) {
        ASSERT_EQ(*game.player1.tiles[i], *startPieces[i]);
        ASSERT_EQ(*game.player2.tiles[i], *startPieces[i]);
    }
}

TEST_F(GameInitializationTest, BoardInit) {
    ASSERT_TRUE(game.board->tiles.empty());
    ASSERT_EQ(game.board->emptyTiles.size(), 1);
    
    auto type = game.board->emptyTiles[{0, 0}].type;
    ASSERT_EQ(type, hge::TileType::EMPTY);
}

TEST_F(GameInitializationTest, GameReset) {
    // game.reset(); // Uncomment this line after implementing the reset function
    ASSERT_TRUE(game.board->tiles.empty());
    ASSERT_EQ(game.board->emptyTiles.size(), 1);
    
    auto type = game.board->emptyTiles[{0, 0}].type;
    ASSERT_EQ(type, hge::TileType::EMPTY);
    ASSERT_EQ(game.player1.tiles.size(), 11);
    ASSERT_EQ(game.player2.tiles.size(), 11);
}