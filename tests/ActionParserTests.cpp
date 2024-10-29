#include <gtest/gtest.h>
#include <filesystem>
#include "BoardDrawable.hpp"
#include "ActionParser.hpp"
#include "BaseBoard.hpp"
#include "Player.hpp"
#include "Game.hpp"
#include "BaseTest.hpp"

namespace hive
{
    class ActionParserTests : public BaseTest
    {
    };


    //tests translating action to notation
    TEST_F(ActionParserTests, actionToNotation)
    {
        ASSERT_TRUE(applyAction(PlaceAction({0, 0}, 'Q')));
        ASSERT_EQ(getLastAction(), "WQ");
        ASSERT_TRUE(applyAction(PlaceAction({1, 0}, 'Q')));
        ASSERT_EQ(getLastAction(), "BQ WQ-");
        ASSERT_TRUE(applyAction(PlaceAction({-1, 0}, 'A')));
        ASSERT_EQ(getLastAction(), "WA1 -WQ");
        ASSERT_TRUE(applyAction(PlaceAction({1, 1}, 'A')));
        ASSERT_EQ(getLastAction(), "BA1 BQ\\");
        ASSERT_TRUE(applyAction(PlaceAction({0, -1}, 'A')));
        ASSERT_TRUE(getLastAction() == "WA2 WA1/" || getLastAction() == "WA2 \\WQ");
        ASSERT_TRUE(applyAction(PlaceAction({2, 0}, 'A')));
        ASSERT_TRUE(getLastAction() == "BA2 BQ-" || getLastAction() == "BA2 BA1/");
        ASSERT_TRUE(applyAction(PlaceAction({-1, 1}, 'G')));
        ASSERT_TRUE(getLastAction() == "WG1 /WQ" || getLastAction() == "WG1 WA1\\");
        ASSERT_TRUE(applyAction(MoveAction({1, 1}, {0, 1})));
        ASSERT_TRUE(getLastAction() == "BA1 /BQ" || getLastAction() == "BA1 WQ\\" || getLastAction() == "BA1 WG1-");
        ASSERT_TRUE(applyAction(PlaceAction({-1, -1}, 'G')));
        ASSERT_TRUE(getLastAction() == "WG2 \\WA1" || getLastAction() == "WG2 -WA2\\");
        ASSERT_TRUE(applyAction(MoveAction({2, 0}, {1, -1})));
        ASSERT_EQ(getGameStatus(), "BLACK_WINS");
    }


    //tests playing game with notation
    TEST_F(ActionParserTests, simpleGame)
    {
        std::vector<std::string> game = {
            "WQ",
            "BQ -WQ",
            "WG1 WQ/",
            "BG1 \\BQ",
            "WG1 /WQ",
            "BG1 WG1\\"};

        playGame(game);
    }
}