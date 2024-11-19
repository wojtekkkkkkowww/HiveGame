#pragma once

#include <gtest/gtest.h>
#include <filesystem>
#include "Game.hpp"
#include "BoardDrawable.hpp"
#include "GameViewer.hpp"
#include "FileOperations.hpp"

namespace hive
{
    class BaseTest : public ::testing::Test, public Game
    {
    public:
        BaseTest() : Game()
        {
            renderTexture.create(800, 800);
        }

        void SetUp() override
        {
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
                if (action.type == "MOVE" && action.position == position)
                {
                    actualMoves.insert(action.newPosition);
                }
            }

            EXPECT_EQ(actualMoves, expectedMoves);
            if(actualMoves != expectedMoves)
            {
                std::cerr << "Empty tiles: ";
                for (const auto &tile : board.emptyTiles)
                {
                    std::cerr <<"(" << tile.x << " " << tile.y << ") ";
                }
                std::cerr << std::endl;

                std::cerr << "Expected moves: ";
                for (const auto &move : expectedMoves)
                {
                    std::cerr <<"(" << move.x << " " << move.y << ") ";
                }
                std::cerr << std::endl;

                std::cerr << "Actual moves:   ";
                for (const auto &move : actualMoves)
                {
                    std::cerr <<"(" << move.x << " " << move.y << ") ";
                }
                std::cerr << std::endl;
            }
        }

        void TearDown() override
        {
            saveGame();
        }

        void saveBoardAsPng(const std::string &filePath)
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

        void saveBoardState()
        {
            int turnCounter1 = players['W']->turnCounter;
            int turnCounter2 = players['B']->turnCounter;
            int turnCounter = turnCounter1 + turnCounter2;

            const ::testing::TestInfo *test_info = ::testing::UnitTest::GetInstance()->current_test_info();
            std::string testName = test_info->name();

            std::string filePath = testName + "/" + std::to_string(turnCounter) + ".png";
            saveBoardAsPng(filePath);
        }

    protected:
        sf::RenderTexture renderTexture;
        BoardDrawable boardDrawable = BoardDrawable(board, 32.0f);

        void saveGame()
        {
            const ::testing::TestInfo *test_info = ::testing::UnitTest::GetInstance()->current_test_info();
            std::string testName = test_info->name();
            saveToFile(testName, actionStrings);
        }
    };
}