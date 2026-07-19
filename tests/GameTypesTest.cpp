#include "GameTypes.hpp"
#include <gtest/gtest.h>

namespace hive
{
TEST(GameTypesTest, ConvertsPieceTypesAtNotationBoundary)
{
    EXPECT_EQ(pieceTypeFromChar('Q'), PieceType::Queen);
    EXPECT_EQ(pieceTypeFromChar('A'), PieceType::Ant);
    EXPECT_EQ(pieceTypeFromChar('S'), PieceType::Spider);
    EXPECT_EQ(pieceTypeFromChar('G'), PieceType::Grasshopper);
    EXPECT_EQ(pieceTypeFromChar('B'), PieceType::Beetle);

    EXPECT_EQ(toChar(PieceType::Queen), 'Q');
    EXPECT_THROW(static_cast<void>(pieceTypeFromChar('X')), std::invalid_argument);
}

TEST(GameTypesTest, ConvertsPlayerColorsAtProtocolBoundary)
{
    EXPECT_EQ(playerColorFromChar('W'), PlayerColor::White);
    EXPECT_EQ(playerColorFromChar('B'), PlayerColor::Black);
    EXPECT_EQ(toChar(PlayerColor::White), 'W');
    EXPECT_EQ(opposite(PlayerColor::White), PlayerColor::Black);
    EXPECT_EQ(opposite(PlayerColor::Black), PlayerColor::White);
    EXPECT_THROW(static_cast<void>(playerColorFromChar('X')), std::invalid_argument);
}

TEST(GameTypesTest, ConvertsStatusOnlyForPresentation)
{
    EXPECT_EQ(toString(GameStatus::Playing), "PLAYING");
    EXPECT_EQ(toString(GameStatus::WhiteWins), "WHITE_WINS");
    EXPECT_EQ(toString(GameStatus::BlackWins), "BLACK_WINS");
    EXPECT_EQ(toString(GameStatus::Draw), "DRAW");
}
} // namespace hive
