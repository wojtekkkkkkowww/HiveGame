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

TEST_F(GameInitializationTest, LoadingStateReplacesPreviousGameAndRestoresInventory)
{
    ASSERT_TRUE(game.applyAction(hive::makePlaceAction({0, 0}, hive::PieceType::Queen)));
    ASSERT_TRUE(game.applyAction(hive::makePlaceAction({1, 0}, hive::PieceType::Queen)));

    std::map<hive::Position, std::deque<hive::Tile>> state = {
        {{4, -2}, {hive::Tile("WA1")}},
    };
    game.startGameFromState(state, hive::PlayerColor::White);

    EXPECT_EQ(game.board.getPieceCount(), 1U);
    EXPECT_TRUE(game.board.isEmpty({0, 0}));
    EXPECT_EQ(game.board.getTile({4, -2}).notation, "WA1");
    EXPECT_EQ(game.board.whiteQueen, hive::invalidPosition);
    EXPECT_EQ(game.board.blackQueen, hive::invalidPosition);
    EXPECT_EQ(game.players[hive::PlayerColor::White].getTileCount(hive::PieceType::Ant), 2);
    EXPECT_EQ(game.getPlyCount(), 0U);
    EXPECT_EQ(game.getGameStatus(), hive::GameStatus::Playing);
}
