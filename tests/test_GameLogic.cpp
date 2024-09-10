#include <vector>
#include <memory>
#include <algorithm>
#include <gtest/gtest.h>
#include "HiveGameEngine.h"


class GameLogicTest : public ::testing::Test {
protected:
    void SetUp() override {    }

    hge::Game game;
};

TEST_F(GameLogicTest, GameConstructor) {
    SUCCEED();
}

bool compareTiles(const std::shared_ptr<hge::Tile>& a, const std::shared_ptr<hge::Tile>& b) {
    return *a == *b;
}

TEST_F(GameLogicTest, PiecesAtStart) {
    /*
    check available pieces for both players
    */
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
    /*
    check if both players have the same pieces
    */
    ASSERT_EQ(game.player1.tiles.size(), startPieces.size());
    ASSERT_EQ(game.player2.tiles.size(), startPieces.size());

    for (size_t i = 0; i < startPieces.size(); ++i) {
        ASSERT_TRUE(compareTiles(game.player1.tiles[i], startPieces[i]));
        ASSERT_TRUE(compareTiles(game.player2.tiles[i], startPieces[i]));
    }
}

TEST_F(GameLogicTest, BoardInit) {
    
    /*
    check if board has no pieces after game start
    */
    ASSERT_TRUE(game.board->tiles.empty());
    std::cout << game.board->emptyTiles.size() << std::endl;
    ASSERT_EQ(game.board->emptyTiles.size(), 1);

    /*
    check that board has only one empty tile at the start 
    */
    auto tileType = game.board->emptyTiles[{0,0}].type;
    ASSERT_EQ(tileType, hge::TileType::EMPTY );
}

TEST_F(GameLogicTest, QueenBeeAvailableMovesAtStart) {

    auto queenBee = std::make_shared<hge::QueenBee>(game.board);  
    game.board->addTile({0,0},queenBee);
    auto availableMoves = queenBee->getAvailableMoves();
    
    ASSERT_EQ(availableMoves.size(), 6);
    std::set<std::pair<int, int>> expectedMoves = {
        {1, -1}, {1, 0}, {0, 1}, {-1, 1}, {-1, 0}, {0, -1}
    };
    ASSERT_EQ(availableMoves, expectedMoves);
}



class TileTest : public ::testing::Test {
protected:
    hge::EmptyTile tile;
};

TEST_F(TileTest, GetNeighboursAtOrigin) {
    tile.setPosition({0, 0});
    std::set<std::pair<int, int>> expectedNeighbours = {
        {1, -1}, {1, 0}, {0, 1}, {-1, 1}, {-1, 0}, {0, -1}
    };
    ASSERT_EQ(tile.getNeighbours(), expectedNeighbours);
}

TEST_F(TileTest, GetNeighboursAtPosition) {
    tile.setPosition({2, 0});
    std::set<std::pair<int, int>> expectedNeighbours = {
        {3, -1}, {3, 0}, {2, 1}, {1, 1}, {1, 0}, {2, -1}
    };
    ASSERT_EQ(tile.getNeighbours(), expectedNeighbours);
}