#pragma once

#include <gtest/gtest.h>
#include <filesystem>
#include "Game.hpp"
#include "BoardDrawable.hpp"

namespace hive
{
    class BaseTest : public ::testing::Test, public Game
    {
    public:
        BaseTest() : Game()
        {
        }

        void SetUp() override
        {
            Game::startNewGame();
            if (saveState)
                renderTexture.create(800, 800);
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

        bool applyAction(const std::string &notation)
        {
            bool result = Game::applyAction(notation);

            if (saveState)
                saveBoardState();

            return result;
        }

        bool applyAction(const Action &action)
        {
            bool result = Game::applyAction(action);

            if (saveState)
                saveBoardState();

            return result;
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

        void saveBoardState()
        {
            int turnCounter1 = players['W'].turnCounter;
            int turnCounter2 = players['B'].turnCounter;
            int turnCounter = turnCounter1 + turnCounter2;

            const ::testing::TestInfo *test_info = ::testing::UnitTest::GetInstance()->current_test_info();
            std::string testName = test_info->name();

            std::string filePath = testName + "/" + std::to_string(turnCounter) + ".png";
            saveBoardAsPng(filePath);
        }

    protected:
        sf::RenderTexture renderTexture;
        BoardDrawable boardDrawable = BoardDrawable(board, 32.0f);
        bool saveState = false;
    };
}