#include <set>
#include <gtest/gtest.h>
#include <iostream>
#include <filesystem>
#include <thread>
#include "Game.hpp"
#include "BoardDrawable.hpp"

using namespace hive;

class TileMovementTest : public ::testing::Test, public Game
{
public:
    sf::RenderTexture renderTexture;
    BoardDrawable boardDrawable = BoardDrawable(board, 32.0f);

    TileMovementTest() : Game()
    {
        renderTexture.create(800, 600);
    }

    void checkMoves(Position position, const std::set<Position> &expectedMoves)
    {
        try
        {
            auto actions = getAvailableActions();
            std::set<Position> actualMoves{};

            for (const auto &action : actions)
            {
                if (action.type == "MOVE" && action.position == position)
                {
                    actualMoves.insert(action.newPosition);
                }
            }

            ASSERT_EQ(actualMoves, expectedMoves);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    void saveBoardAsPng(const std::string &filePath, float hexSize = 32.0f)
    {
        std::filesystem::path path(filePath);
        std::filesystem::path directory = path.parent_path();

        if (!directory.empty() && !std::filesystem::exists(directory))
        {
            std::filesystem::create_directories(directory);
        }

        boardDrawable.update();
        renderTexture.clear(sf::Color::White);
        renderTexture.draw(boardDrawable);
        renderTexture.display();

        if (renderTexture.getTexture().copyToImage().saveToFile(filePath))
        {
            std::cout << "Saved board state to " << filePath << std::endl;
        }
    }

    bool checkCounters(char tileType, int expectedCount)
    {
        bool result = applyAction(PlaceAction({0, 0}, tileType));
        for (int i = 1; i <= expectedCount; i++)
        {
            result = applyAction(PlaceAction({i, 0}, tileType));
            if (!result)
            {
                break;
            }
            result = applyAction(PlaceAction({-i, 0}, tileType));
        }
        return result;
    }

    bool applyAction(const Action &action)
    {
        bool result = Game::applyAction(action);
        int turnCounter1 = players["WHITE"].turnCounter;
        int turnCounter2 = players["BLACK"].turnCounter;
        int turnCounter = turnCounter1 + turnCounter2;

        const ::testing::TestInfo *test_info = ::testing::UnitTest::GetInstance()->current_test_info();
        std::string testName = test_info->name();

        std::string filePath = testName + "/" + std::to_string(turnCounter) + ".png";
        saveBoardAsPng(filePath);

        return result;
    }
};

TEST_F(TileMovementTest, QueenBeeAvailableMovesAtStart)
{
    Tile queenBee('Q', "WHITE");
    board.addTile({0, 0}, queenBee);
    auto availableMoves = board.getAvailableMoves(queenBee);

    std::set<Position> expectedMoves = {
        {1, -1}, {1, 0}, {0, 1}, {-1, 1}, {-1, 0}, {0, -1}};

    ASSERT_EQ(availableMoves.size(), 6);

    ASSERT_EQ(availableMoves, expectedMoves);

    saveBoardAsPng("QueenBeeAvailableMovesAtStart/1.png");
}

TEST_F(TileMovementTest, PlaceNewTile)
{
    PlaceAction action{{0, 0}, 'A'};
    applyAction(action);

    auto tile = board.getTile({0, 0});
    ASSERT_EQ(tile.type, 'A');
    ASSERT_EQ(tile.color, "WHITE");
    Position position = {0, 0};
    ASSERT_EQ(tile.position, position);
}

TEST_F(TileMovementTest, MoveExistingTile)
{
    ASSERT_EQ(getCurrentTurn(), "WHITE");
    ASSERT_TRUE(applyAction(PlaceAction({0, 0}, 'Q')));
    ASSERT_EQ(getCurrentTurn(), "BLACK");

    ASSERT_FALSE(applyAction(MoveAction{{0, 0}, {0, 1}}));
    ASSERT_TRUE(applyAction(PlaceAction({1, 0}, 'Q')));
    ASSERT_EQ(getCurrentTurn(), "WHITE");
    ASSERT_TRUE(applyAction(MoveAction({0, 0}, {0, 1})));

    auto tile = board.getTile({0, 1});
    ASSERT_EQ(tile.type, 'Q');
}

TEST_F(TileMovementTest, CountersCheck)
{
    ASSERT_FALSE(checkCounters('A', 3));
    ASSERT_FALSE(checkCounters('G', 3));
    ASSERT_FALSE(checkCounters('S', 2));
    ASSERT_FALSE(checkCounters('B', 2));
    ASSERT_FALSE(checkCounters('Q', 1));
}

TEST_F(TileMovementTest, BlackWins)
{
    ASSERT_TRUE(applyAction(PlaceAction({0, 0}, 'Q')));
    ASSERT_TRUE(applyAction(PlaceAction({1, 0}, 'Q')));
    ASSERT_TRUE(applyAction(PlaceAction({-1, 0}, 'A')));
    ASSERT_TRUE(applyAction(PlaceAction({1, 1}, 'A')));
    ASSERT_TRUE(applyAction(PlaceAction({0, -1}, 'A')));
    ASSERT_TRUE(applyAction(PlaceAction({2, 0}, 'A')));
    ASSERT_TRUE(applyAction(PlaceAction({-1, 1}, 'G')));
    ASSERT_TRUE(applyAction(MoveAction({1, 1}, {0, 1})));
    ASSERT_TRUE(applyAction(PlaceAction({-1, -1}, 'G')));
    ASSERT_TRUE(applyAction(MoveAction({2, 0}, {1, -1})));
    ASSERT_EQ(getGameStatus(), "BLACK_WINS");
}

TEST_F(TileMovementTest, WhiteWins)
{
    ASSERT_TRUE(applyAction(PlaceAction({0, 0}, 'Q')));
    ASSERT_TRUE(applyAction(PlaceAction({1, 0}, 'Q')));
    ASSERT_TRUE(applyAction(PlaceAction({0, -1}, 'A')));
    ASSERT_TRUE(applyAction(PlaceAction({1, 1}, 'A')));
    ASSERT_TRUE(applyAction(PlaceAction({-1, 0}, 'A')));
    ASSERT_TRUE(applyAction(PlaceAction({2, 0}, 'A')));
    ASSERT_TRUE(applyAction(MoveAction({0, -1}, {0, 1})));
    ASSERT_TRUE(applyAction(PlaceAction({2, -1}, 'B')));
    ASSERT_TRUE(applyAction(MoveAction({-1, 0}, {1, -1})));

    ASSERT_EQ(getGameStatus(), "WHITE_WINS");
}

TEST_F(TileMovementTest, Draw)
{
    ASSERT_TRUE(applyAction(PlaceAction({0, 0}, 'Q')));
    ASSERT_TRUE(applyAction(PlaceAction({1, 0}, 'Q')));
    ASSERT_TRUE(applyAction(PlaceAction({-1, 1}, 'A')));
    ASSERT_TRUE(applyAction(PlaceAction({1, 1}, 'A')));
    ASSERT_TRUE(applyAction(PlaceAction({-1, 0}, 'A')));
    ASSERT_TRUE(applyAction(PlaceAction({2, 0}, 'A')));
    ASSERT_TRUE(applyAction(PlaceAction({0, -1}, 'A')));
    ASSERT_TRUE(applyAction(PlaceAction({2, -1}, 'A')));
    ASSERT_TRUE(applyAction(PlaceAction({-1, -1}, 'G')));
    ASSERT_TRUE(applyAction(PlaceAction({2, 1}, 'G')));
    ASSERT_TRUE(applyAction(MoveAction({-1, -1}, {1, -1})));
    ASSERT_TRUE(applyAction(MoveAction({2, 1}, {0, 1})));

    ASSERT_EQ(getGameStatus(), "DRAW");
}

TEST_F(TileMovementTest, GrasshopperMove)
{
    ASSERT_TRUE(applyAction(PlaceAction({0, 0}, 'Q')));
    ASSERT_TRUE(applyAction(PlaceAction({0, -1}, 'Q')));
    Position firstGrasshopperPosition = {0, 1};
    ASSERT_TRUE(applyAction(PlaceAction(firstGrasshopperPosition, 'G')));
    Position secondGrasshopperPosition = {0, -2};
    ASSERT_TRUE(applyAction(PlaceAction(secondGrasshopperPosition, 'G')));

    std::set<Position> expectedMoves = {{0, -3}};
    checkMoves(firstGrasshopperPosition, expectedMoves);

    ASSERT_TRUE(applyAction(MoveAction(firstGrasshopperPosition, {0, -3})));
}

TEST_F(TileMovementTest, BeetleJump)
{
    ASSERT_TRUE(applyAction(PlaceAction({0, 0}, 'Q')));
    ASSERT_TRUE(applyAction(PlaceAction({0, -1}, 'Q')));
    Position beetlePosition = {0, 1};
    ASSERT_TRUE(applyAction(PlaceAction(beetlePosition, 'B')));

    Position beetlePosition2 = {0, -2};
    ASSERT_TRUE(applyAction(PlaceAction(beetlePosition2, 'B')));

    std::set<Position> expectedMoves = {{0, 0}, {-1, 1}, {1, 0}};
    std::cerr << "\033[33mChecking moves\033[0m" << std::endl;

    checkMoves(beetlePosition, expectedMoves);
    ASSERT_TRUE(applyAction(MoveAction(beetlePosition, {0, 0})));

    expectedMoves = {{-1, -1}, {1, -2}, {0, -1}};
    checkMoves(beetlePosition2, expectedMoves);

    ASSERT_TRUE(applyAction(MoveAction(beetlePosition2, {0, -1})));

    ASSERT_TRUE(applyAction(MoveAction({0, 0}, {0, -1})));
    expectedMoves = {};
    checkMoves({0, -1}, expectedMoves);
    ASSERT_TRUE(applyAction(WaitAction()));
    ASSERT_TRUE(applyAction(MoveAction({0, -1}, {0, 0})));
}

TEST_F(TileMovementTest, AntMove)
{
    ASSERT_TRUE(applyAction(PlaceAction({0, 0}, 'Q')));
    ASSERT_TRUE(applyAction(PlaceAction({0, -1}, 'Q')));
    Position antPosition = {0, 1};
    ASSERT_TRUE(applyAction(PlaceAction(antPosition, 'A')));
    ASSERT_TRUE(applyAction(MoveAction({0, -1}, {-1, 0})));

    std::set<Position> expectedMoves = {{-1, 1}, {1, 0}, {1, -1}, {-2, 1}, {-2, 0}, {0, -1}, {-1, -1}};
    checkMoves(antPosition, expectedMoves);
    ASSERT_TRUE(applyAction(MoveAction(antPosition, {-2, 0})));
}

TEST_F(TileMovementTest, SpiderMove)
{
    ASSERT_TRUE(applyAction(PlaceAction({0, 0}, 'Q')));
    ASSERT_TRUE(applyAction(PlaceAction({0, -1}, 'Q')));
    ASSERT_TRUE(applyAction(PlaceAction({0, 1}, 'S')));
    ASSERT_TRUE(applyAction(PlaceAction({0, -2}, 'S')));

    std::set<Position> expectedMoves = {{-1, -1}, {1, -2}};
    checkMoves({0, 1}, expectedMoves);
}

/*
do zrobienia:
jakieś testy z "zaawansowaną" plansza i sprawdzenie dostępnych akcji
*/