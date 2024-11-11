// AiTester.cpp
#include "Game.hpp"
#include "AiAlgorithm.hpp"
#include "RandomAi.hpp"
#include "AlphaBethaSearch.hpp"
#include "Heuristic.hpp"
#include "DataBase.hpp"
#include "FileOperations.hpp"
#include <iostream>
#include <memory>
#include <fstream>
#include <chrono>

#define NUMBER_OF_GAMES 1
#define MAX_NUMBER_OF_MOVES 30

using namespace hive;

void setAi(char *arg, std::unique_ptr<hive::AIAlgorithm> &ai, std::unique_ptr<hive::Game> &game)
{
    if (std::string(arg) == "random")
    {
        ai = std::make_unique<RandomAIAlgorithm>(*game);
    }
    else if (std::string(arg) == "alphabeta")
    {
        ai = std::make_unique<AlphaBetaAI>(*game);
    }
    else
    {
        throw std::runtime_error("Unknown AI");
    }
}

int main(int argc, char *argv[])
{
    auto game = std::make_unique<Game>();
    std::unique_ptr<AIAlgorithm> ai1;
    std::unique_ptr<AIAlgorithm> ai2;

    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <AI1> <AI2>" << std::endl;
        std::cerr << "Implemented Ai: random, mcts, alphabeta" << std::endl;
        return 1;
    }
    setAi(argv[1], ai1, game);
    setAi(argv[2], ai2, game);

    int ai1Wins = 0;
    int ai2Wins = 0;
    int draws = 0;

    //DataBase db("game_results.db");

    for (int i = 0; i < NUMBER_OF_GAMES; ++i)
    {
        game->startNewGame();
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        std::vector<std::string> moves;
        for (int j = 0; j < MAX_NUMBER_OF_MOVES && !game->isGameOver(); ++j)
        {
            Action action;
            if (game->getCurrentTurn() == 'W')
            {
                action = ai1->getNextMove();
                std::cout << "AI 1: " << action << std::endl;
            }
            else
            {
                action = ai2->getNextMove();
                std::cout << "AI 2: " << action << std::endl;
                if(ai2->getName() == "AlphaBetaAI")
                {
                    std::cout << "Nodes number: " << dynamic_cast<AlphaBetaAI*>(ai2.get())->NodesNumber << std::endl;
                }
            }

            game->applyAction(action);
            moves.push_back(game->getLastAction());
            std::cout << "Move: " << moves.back() << std::endl;
        }
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Game Time = " << (end - begin).count() << std::endl;
        saveToFile(std::to_string(i), moves);

        std::string result = game->getGameStatus();
        std::string winner;
        if (result == "WHITE_WINS")
        {
            ai1Wins++;
            winner = ai1->getName();
        }
        else if (result == "BLACK_WINS")
        {
            ai2Wins++;
            winner = ai2->getName();
        }
        else
        {
            draws++;
            winner = "Draw";
        }
    }

    std::cout << "AI 1 Wins: " << ai1Wins << std::endl;
    std::cout << "AI 2 Wins: " << ai2Wins << std::endl;
    std::cout << "Draws: " << draws << std::endl;

    return 0;
}