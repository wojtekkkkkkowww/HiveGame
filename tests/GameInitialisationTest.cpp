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
    const auto &board = game.getBoard();

    ASSERT_EQ(board.getTileCount(), 0);

    ASSERT_TRUE(board.emptyTiles.find({0, 0}) != board.emptyTiles.end());
    ASSERT_TRUE(board.emptyTiles.size() == 1);
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
    const auto &board = game.getBoard();
    const auto &players = game.getPlayers();

    EXPECT_EQ(board.getPieceCount(), 1U);
    EXPECT_TRUE(board.isEmpty({0, 0}));
    EXPECT_EQ(board.getTile({4, -2}).notation, "WA1");
    EXPECT_EQ(board.whiteQueen, hive::invalidPosition);
    EXPECT_EQ(board.blackQueen, hive::invalidPosition);
    EXPECT_EQ(players[hive::PlayerColor::White].getTileCount(hive::PieceType::Ant), 2);
    EXPECT_EQ(game.getPlyCount(), 0U);
    EXPECT_EQ(game.getGameStatus(), hive::GameStatus::Playing);
}
