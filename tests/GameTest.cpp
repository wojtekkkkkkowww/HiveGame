#include "Game.hpp"
#include "BaseTest.hpp"
#include <filesystem>
#include <gtest/gtest.h>
#include <iostream>
#include <set>
#include <stdexcept>
#include <thread>

using namespace hive;

class GameTest : public BaseTest
{
  public:
    GameTest() : BaseTest() {}

    bool checkCounters(PieceType pieceType, int expectedCount)
    {
        bool result = applyAction(makePlaceAction({.x = 0, .y = 0}, pieceType));
        for (int i = 1; i <= expectedCount; i++)
        {
            result = applyAction(makePlaceAction({.x = i, .y = 0}, pieceType));
            if (!result)
            {
                break;
            }
            result = applyAction(makePlaceAction({.x = -i, .y = 0}, pieceType));
        }
        return result;
    }
};

namespace
{
std::vector<Action> blackWins = {makePlaceAction({0, 0}, PieceType::Queen),
                                 makePlaceAction({1, 0}, PieceType::Queen),
                                 makePlaceAction({-1, 0}, PieceType::Ant),
                                 makePlaceAction({1, 1}, PieceType::Ant),
                                 makePlaceAction({0, -1}, PieceType::Ant),
                                 makePlaceAction({2, 0}, PieceType::Ant),
                                 makePlaceAction({-1, 1}, PieceType::Grasshopper),
                                 makeMoveAction({1, 1}, {0, 1}),
                                 makePlaceAction({-1, -1}, PieceType::Grasshopper),
                                 makeMoveAction({2, 0}, {1, -1})

};

std::vector<Action> whiteWins = {
    makePlaceAction({0, 0}, PieceType::Queen), makePlaceAction({1, 0}, PieceType::Queen),
    makePlaceAction({0, -1}, PieceType::Ant),  makePlaceAction({1, 1}, PieceType::Ant),
    makePlaceAction({-1, 0}, PieceType::Ant),  makePlaceAction({2, 0}, PieceType::Ant),
    makeMoveAction({0, -1}, {0, 1}),           makePlaceAction({2, -1}, PieceType::Beetle),
    makeMoveAction({-1, 0}, {1, -1})

};

std::vector<Action> draw = {makePlaceAction({0, 0}, PieceType::Queen),
                            makePlaceAction({1, 0}, PieceType::Queen),
                            makePlaceAction({-1, 1}, PieceType::Ant),
                            makePlaceAction({1, 1}, PieceType::Ant),
                            makePlaceAction({-1, 0}, PieceType::Ant),
                            makePlaceAction({2, 0}, PieceType::Ant),
                            makePlaceAction({0, -1}, PieceType::Ant),
                            makePlaceAction({2, -1}, PieceType::Ant),
                            makePlaceAction({-1, -1}, PieceType::Grasshopper),
                            makePlaceAction({2, 1}, PieceType::Grasshopper),
                            makeMoveAction({-1, -1}, {1, -1}),
                            makeMoveAction({2, 1}, {0, 1})

};
} // namespace

TEST_F(GameTest, PlaceNewTile)
{
    applyAction(makePlaceAction({.x = 0, .y = 0}, PieceType::Ant));

    const auto &tile = board.getTile({.x = 0, .y = 0});
    ASSERT_EQ(tile.type, PieceType::Ant);
    ASSERT_EQ(tile.color, PlayerColor::White);
    Position position = {.x = 0, .y = 0};
    ASSERT_EQ(tile.position, position);
}

TEST_F(GameTest, MoveExistingTile)
{
    ASSERT_EQ(getCurrentTurn(), PlayerColor::White);
    ASSERT_TRUE(applyAction(makePlaceAction({0, 0}, PieceType::Queen)));
    ASSERT_EQ(getCurrentTurn(), PlayerColor::Black);

    ASSERT_FALSE(applyAction(makeMoveAction({0, 0}, {0, 1})));
    ASSERT_TRUE(applyAction(makePlaceAction({1, 0}, PieceType::Queen)));
    ASSERT_EQ(getCurrentTurn(), PlayerColor::White);
    ASSERT_TRUE(applyAction(makeMoveAction({0, 0}, {0, 1})));

    const auto &tile = board.getTile({0, 1});
    ASSERT_EQ(tile.type, PieceType::Queen);
}

TEST_F(GameTest, CountersCheck)
{
    ASSERT_FALSE(checkCounters(PieceType::Ant, 3));
    ASSERT_FALSE(checkCounters(PieceType::Grasshopper, 3));
    ASSERT_FALSE(checkCounters(PieceType::Spider, 2));
    ASSERT_FALSE(checkCounters(PieceType::Beetle, 2));
    ASSERT_FALSE(checkCounters(PieceType::Queen, 1));
}

TEST_F(GameTest, BlackWins)
{
    playGame(blackWins);
    ASSERT_EQ(getGameStatus(), GameStatus::BlackWins);
}

TEST_F(GameTest, WhiteWins)
{
    playGame(whiteWins);

    ASSERT_EQ(getGameStatus(), GameStatus::WhiteWins);
}

TEST_F(GameTest, Draw)
{
    playGame(draw);

    ASSERT_EQ(getGameStatus(), GameStatus::Draw);
}

TEST_F(GameTest, RejectedActionDoesNotChangeHistory)
{
    ASSERT_TRUE(applyAction(makePlaceAction({0, 0}, PieceType::Queen)));
    const std::string lastValidAction = getLastAction();

    ASSERT_FALSE(applyAction(makeMoveAction({0, 0}, {0, 1})));
    EXPECT_EQ(getLastAction(), lastValidAction);
    EXPECT_EQ(getPlyCount(), 1U);
}

TEST_F(GameTest, RevertingPlacedQueenRestoresQueenState)
{
    ASSERT_TRUE(applyAction(makePlaceAction({0, 0}, PieceType::Queen)));
    ASSERT_EQ(board.whiteQueen, (Position{0, 0}));

    revertAction();

    EXPECT_EQ(board.whiteQueen, invalidPosition);
    EXPECT_EQ(board.blackQueen, invalidPosition);
    EXPECT_FALSE(players[PlayerColor::White].queenPlaced);
    EXPECT_EQ(getCurrentTurn(), PlayerColor::White);
    EXPECT_EQ(getPlyCount(), 0U);
}

TEST_F(GameTest, RevertingWithoutActionsKeepsInitialState)
{
    EXPECT_FALSE(canRevertAction());
    revertAction();
    revertValidAction();

    EXPECT_EQ(getCurrentTurn(), PlayerColor::White);
    EXPECT_EQ(getPlyCount(), 0U);
    EXPECT_EQ(getGameStatus(), GameStatus::Playing);
    EXPECT_TRUE(board.isEmpty({0, 0}));
    EXPECT_THROW(getLastAction(), std::logic_error);
}

TEST(GameMoveLimitTest, DrawsAtConfiguredPlyLimitAndCanBeReverted)
{
    Game game(2);

    ASSERT_TRUE(game.applyAction(makePlaceAction({0, 0}, PieceType::Queen)));
    EXPECT_FALSE(game.isGameOver());
    ASSERT_TRUE(game.applyAction(makePlaceAction({1, 0}, PieceType::Queen)));

    EXPECT_TRUE(game.isGameOver());
    EXPECT_EQ(game.getGameStatus(), GameStatus::Draw);
    EXPECT_EQ(game.getPlyCount(), 2U);

    game.revertAction();
    EXPECT_FALSE(game.isGameOver());
    EXPECT_EQ(game.getGameStatus(), GameStatus::Playing);
    EXPECT_EQ(game.getPlyCount(), 1U);
}

TEST(GameMoveLimitTest, RejectsZeroLimit) { EXPECT_THROW(Game(0), std::invalid_argument); }

TEST_F(GameTest, SearchApplyAndUndoPreserveRecordedHistory)
{
    ASSERT_TRUE(applyAction(makePlaceAction({0, 0}, PieceType::Queen)));
    const auto lastRecordedAction = getLastAction();
    const auto turnBeforeSearch = getCurrentTurn();
    const auto plyBeforeSearch = getPlyCount();
    const auto searchAction = availableActions.front();

    applyValidAction(searchAction);
    revertValidAction();

    EXPECT_EQ(getLastAction(), lastRecordedAction);
    EXPECT_EQ(getCurrentTurn(), turnBeforeSearch);
    EXPECT_EQ(getPlyCount(), plyBeforeSearch);
}
