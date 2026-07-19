#pragma once

#include "FileOperations.hpp"
#include "Game.hpp"
#include <gtest/gtest.h>

namespace hive
{
class BaseTest : public ::testing::Test, public Game
{
  public:
    BaseTest() : Game() {}

    void SetUp() override
    {
        hive::setOutputDir("build/tests/games");
        hive::clearOutputFiles();

        Game::startNewGame();
    }

    void playGame(const std::vector<std::string> &game)
    {
        for (const auto &action : game)
        {
            std::cerr << "Applying action " << action << std::endl;
            ASSERT_TRUE(applyAction(action));
        }
    }

    void playGame(const std::vector<Action> &game)
    {
        for (const auto &action : game)
        {
            std::cerr << "Applying action " << action << std::endl;
            ASSERT_TRUE(applyAction(action));
        }
    }

    void checkMoves(Position position, const std::set<Position> &expectedMoves)
    {
        auto actions = avaliableActions;
        std::set<Position> actualMoves{};

        for (const auto &action : actions)
        {
            if (action.type == ActionType::MOVE && action.position == position)
            {
                actualMoves.insert(action.newPosition);
            }
        }

        EXPECT_EQ(actualMoves, expectedMoves);
        if (actualMoves != expectedMoves)
        {
            std::cerr << "Empty tiles: ";
            for (const auto &tile : board.emptyTiles)
            {
                std::cerr << "(" << tile.x << " " << tile.y << ") ";
            }
            std::cerr << std::endl;

            std::cerr << "Expected moves: ";
            for (const auto &move : expectedMoves)
            {
                std::cerr << "(" << move.x << " " << move.y << ") ";
            }
            std::cerr << std::endl;

            std::cerr << "Actual moves:   ";
            for (const auto &move : actualMoves)
            {
                std::cerr << "(" << move.x << " " << move.y << ") ";
            }
            std::cerr << std::endl;
        }
    }

    void TearDown() override { saveGame(); }

    void saveBoardState()
    {
        int turnCounter1 = players[PlayerColor::White].turnCounter;
        int turnCounter2 = players[PlayerColor::Black].turnCounter;
        int turnCounter = turnCounter1 + turnCounter2;

        const ::testing::TestInfo *test_info =
            ::testing::UnitTest::GetInstance()->current_test_info();
        std::string testName = test_info->name();

        std::string filePath = testName + "/" + std::to_string(turnCounter) + ".png";
        saveToFile(filePath, actionStrings);
    }

  protected:
    void saveGame()
    {
        const ::testing::TestInfo *test_info =
            ::testing::UnitTest::GetInstance()->current_test_info();
        std::string testName = test_info->name();
        saveToFile(testName, actionStrings);
    }
};
} // namespace hive
