#include "BaseTest.hpp"
#include <gtest/gtest.h>

namespace hive
{
class ActionParserTests : public BaseTest
{
};

// tests translating action to notation
TEST_F(ActionParserTests, actionToNotation)
{
    ASSERT_TRUE(applyAction(makePlaceAction({0, 0}, PieceType::Queen)));
    ASSERT_EQ(getLastAction(), "WQ");
    ASSERT_TRUE(applyAction(makePlaceAction({1, 0}, PieceType::Queen)));
    ASSERT_EQ(getLastAction(), "BQ WQ-");
    ASSERT_TRUE(applyAction(makePlaceAction({-1, 0}, PieceType::Ant)));
    ASSERT_EQ(getLastAction(), "WA1 -WQ");
    ASSERT_TRUE(applyAction(makePlaceAction({1, 1}, PieceType::Ant)));
    ASSERT_EQ(getLastAction(), "BA1 BQ\\");
    ASSERT_TRUE(applyAction(makePlaceAction({0, -1}, PieceType::Ant)));
    ASSERT_TRUE(getLastAction() == "WA2 WA1/" || getLastAction() == "WA2 \\WQ");
    ASSERT_TRUE(applyAction(makePlaceAction({2, 0}, PieceType::Ant)));
    ASSERT_TRUE(getLastAction() == "BA2 BQ-" || getLastAction() == "BA2 BA1/");
    ASSERT_TRUE(applyAction(makePlaceAction({-1, 1}, PieceType::Grasshopper)));
    ASSERT_TRUE(getLastAction() == "WG1 /WQ" || getLastAction() == "WG1 WA1\\");
    ASSERT_TRUE(applyAction(makeMoveAction({1, 1}, {0, 1})));
    ASSERT_TRUE(getLastAction() == "BA1 /BQ" || getLastAction() == "BA1 WQ\\" ||
                getLastAction() == "BA1 WG1-");
    ASSERT_TRUE(applyAction(makePlaceAction({-1, -1}, PieceType::Grasshopper)));
    ASSERT_TRUE(getLastAction() == "WG2 \\WA1" || getLastAction() == "WG2 -WA2\\");
    ASSERT_TRUE(applyAction(makeMoveAction({2, 0}, {1, -1})));
    ASSERT_EQ(getGameStatus(), GameStatus::BlackWins);
}

// tests playing game with notation
TEST_F(ActionParserTests, simpleGame)
{
    std::vector<std::string> game = {"WQ", "BQ -WQ", "WG1 WQ/", "BG1 \\BQ", "WG1 /WQ", "BG1 WG1\\"};

    playGame(game);
}

TEST_F(ActionParserTests, RejectsPartialAndUnknownNotation)
{
    const auto invalidNotationResult = applyActionExpected("prefix WQ suffix");
    ASSERT_FALSE(invalidNotationResult.has_value());
    EXPECT_EQ(invalidNotationResult.error(), ApplyActionError::InvalidNotation);

    EXPECT_FALSE(applyAction("prefix WQ suffix"));
    EXPECT_FALSE(applyAction("not a move"));
    EXPECT_EQ(getPlyCount(), 0U);
    EXPECT_TRUE(board.isEmpty({0, 0}));
}
} // namespace hive
