#include <gtest/gtest.h>
#include "BaseTest.hpp"

using namespace hive;

class ActionGeneratorTest : public BaseTest
{
public:
    ActionGeneratorTest() : BaseTest()
    {
        // Optionally enable saving state for debugging
        // saveState = true;
    }

    void checkFirstMove(const std::string &move)
    {
        applyAction(move);
        ASSERT_EQ(avaliableActions.size(), 5 * 6);
        revertAction();
        ASSERT_EQ(avaliableActions.size(), 5);
    }
};

namespace
{
    std::vector<std::string> game = {
        "WG1",
        "BG1 /WG1",
        "WQ WG1-",
        "BA1 /BG1",
        "WA1 \\WQ",
        "BA2 /BA1",
        "WA1 /BA2",
        "BQ \\BA1",
        "WS1 \\WQ",
        "BS1 -BQ",
        "WS1 \\BQ",
        "BS1 WS1/",
        "WB1 -WS1",
        "BB1 /BQ",
        "WB1 -BQ",
        "BB1 WB1.",
        "WG2 WQ-",
        "BB2 BG1\\",
        "WG2 BQ/",
        "BG1 \\WQ",
        "WG3 /WA1",
        "BS2 BG1/",
        "WG3 BQ-",
        "BG2 /BB1",
        "WQ BS2\\",
        "BA3 BS2/",
        "WQ BA3\\",
        "BG3 \\BS2",
        "WQ BA3-",
    };

    Position blackQueen = {-2, 1};
    std::set<Position> blackQueenMoves = {};

    Position blackGrasshopper = {2, -3};
    std::set<Position> blackGrasshopperMoves = {{2, -1}, {5, -3}};

    Position blackSpider = {-1, -1};
    std::set<Position> blackSpiderMoves = {{1, -3}, {-4, 1}};

    Position blackBeetle1 = {-3, 1};
    std::set<Position> blackBeetleMoves1 = {{-2, 0}, {-2, 1}, {-4, 2}, {-3, 2}, {-4, 1}, {-3, 0}};

    Position blackBeetle2 = {-1, 2};
    std::set<Position> blackBeetleMoves2 = {{0, 1}, {-2, 3}, {-1, 1}, {-2, 2}};

    std::vector<std::string> whitegame = []
    {
        std::vector<std::string> temp = game;
        temp.push_back("BB2 BA1.");
        return temp;
    }();

    std::set<Position> whiteQueenMoves = {{4, -4}, {3, -2}};
    Position whiteQueen = {4, -3};

    std::set<Position> whiteAntMoves =
        {{2, -4}, {3, -4}, {4, -4}, {5, -4}, {5, -3}, {4, -2}, {3, -2}, {2, -1}, {1, 0}, {0, 1}, {-1, 2}, {-2, 3}, {-3, 4}, {-4, 3}, {-5, 3}, {-5, 2}, {-4, 1}, {-3, 0}, {-2, -1}, {-1, -2}, {0, -2}, {0, -1}, {1, -2}, {1, -3}};

    Position whiteAnt = {-4, 4};

    Position whiteSpider = {-2, 0};
    std::set<Position> whiteSpiderMoves = {
        {0, -2}, {-5, 2}};

    Position whiteGrasshopper1 = {-1, 0};
    std::set<Position> whiteGrasshopper1moves = {
        {-1, 2}, {-1, -2}, {-3, 2}, {-3, 0}, {1, 0}};

    Position whiteGrasshopper2 = {0, 0};
    std::set<Position> whiteGrasshopper2moves = {};

    Position whiteGrasshopper3 = {-1, 1};
    std::set<Position> whiteGrasshopper3moves = {
        {-4, 1}, {-1, -2}, {4, -4}, {-5, 5}};
}

TEST_F(ActionGeneratorTest, GeneratingActionsBlackTurn)
{
    playGame(game);
    checkMoves(blackQueen, blackQueenMoves);
    checkMoves(blackGrasshopper, blackGrasshopperMoves);
    checkMoves(blackSpider, blackSpiderMoves);
    checkMoves(blackBeetle1, blackBeetleMoves1);
    checkMoves(blackBeetle2, blackBeetleMoves2);
}

TEST_F(ActionGeneratorTest, GeneratingActionsWhiteTurn)
{
    playGame(whitegame);
    checkMoves(whiteQueen, whiteQueenMoves);
    checkMoves(whiteAnt, whiteAntMoves);
    checkMoves(whiteSpider, whiteSpiderMoves);
    checkMoves(whiteGrasshopper1, whiteGrasshopper1moves);
    checkMoves(whiteGrasshopper2, whiteGrasshopper2moves);
    checkMoves(whiteGrasshopper3, whiteGrasshopper3moves);
}

TEST_F(ActionGeneratorTest, FirstMove)
{
    checkFirstMove("WQ");
    checkFirstMove("WG1");
    checkFirstMove("WA1");
    checkFirstMove("WS1");
    checkFirstMove("WB1");
}

TEST_F(ActionGeneratorTest, SecondMove)
{
    applyAction("WQ");
    applyAction("BG1 WQ-");
    int piecesLeft = 5 - 1;
    int avaliablePositions = 3;
    int avaliableQueenMoves = 2;
    ASSERT_EQ(avaliableActions.size(), piecesLeft * avaliablePositions + avaliableQueenMoves);
}