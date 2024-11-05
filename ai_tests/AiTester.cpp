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

#define NUMBER_OF_GAMES 1
#define MAX_NUMBER_OF_MOVES 20

using namespace hive;

int main()
{
    auto game = std::make_unique<Game>();

    RandomAIAlgorithm ai1(*game);
    AlphaBetaAI alphaBetaAI(*game);
    alphaBetaAI.addHeuristic(std::make_unique<PieceCountHeuristic>(), 1.0);
    alphaBetaAI.addHeuristic(std::make_unique<TilesOroundOpponentQueen>(), 2.0);
    alphaBetaAI.addHeuristic(std::make_unique<QueenAvailableMoves>(), 1.0);
    alphaBetaAI.addHeuristic(std::make_unique<WinLoseHeuristic>(), 5000);
    alphaBetaAI.addHeuristic(std::make_unique<PlacingQueenHeuristic>(), 1000);

    int ai1Wins = 0;
    int ai2Wins = 0;
    int draws = 0;

    DataBase db("game_results.db");

    for (int i = 0; i < NUMBER_OF_GAMES; ++i)
    {
        game->startNewGame();

        std::vector<std::string> moves;
        for (int j = 0; j < MAX_NUMBER_OF_MOVES && !game->isGameOver(); ++j)
        {
            Action action;
            if (game->getCurrentTurn() == 'W')
            {
                action = ai1.getNextMove();
                std::cout << "AI 1: " << action << std::endl;
            }
            else
            {
                action = alphaBetaAI.getNextMove();
                std::cout << "AI 2: " << action << std::endl;
                std::cout << "branching factor: " << game->getAvailableActions().size() << std::endl;
            }

            game->applyAction(action);
            moves.push_back(game->getLastAction());
            std::cout << "Move: " << moves.back() << std::endl;
        }
        saveToFile(std::to_string(i), moves);

        std::string result = game->getGameStatus();
        std::string winner;
        if (result == "WHITE_WINS")
        {
            ai1Wins++;
            winner = ai1.getName();
        }
        else if (result == "BLACK_WINS")
        {
            ai2Wins++;
            winner = alphaBetaAI.getName();
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