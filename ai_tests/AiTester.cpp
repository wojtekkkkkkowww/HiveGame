// AiTester.cpp
#include "Game.hpp"
#include "AiAlgorithm.hpp"
#include "RandomAi.hpp"
#include "DataBase.hpp"
#include "FileOperations.hpp"
#include <iostream>
#include <memory>
#include <fstream>

#define NUMBER_OF_GAMES 10
#define MAX_NUMBER_OF_MOVES 100

using namespace hive;



int main()
{
    Game game;

    RandomAIAlgorithm ai1(game);
    RandomAIAlgorithm ai2(game);

    int ai1Wins = 0;
    int ai2Wins = 0;
    int draws = 0;

    DataBase db("game_results.db");

    for (int i = 0; i < NUMBER_OF_GAMES; ++i)
    {
        game.startNewGame();
        for(const auto& a : game.getAvailableActions()){
            if(a.type == "WAIT")
                std::cerr << "życie nie ma sensu "<< std::endl;
        }
        std::vector<std::string> moves;
        for (int j = 0; j < MAX_NUMBER_OF_MOVES && !game.isGameOver(); ++j)
        {
            std::cout << "Game: " << i << " Move: " << j << std::endl;
            Action action;
            if (game.getCurrentTurn() == 'W')
            {
                std::cerr << "AI 1 turn" << std::endl;
                action = ai1.getNextMove();
            }
            else
            {   
                std::cerr << "AI 2 turn" << std::endl;
                action = ai2.getNextMove();
            }

            game.applyAction(action);
            
            std::cerr << "Applied action: " << action << std::endl;
            std ::cerr << "Applied action: " << game.getLastAction() << std::endl;
            moves.push_back(game.getLastAction());
        }
        saveToFile(std::to_string(i),moves); 

        std::string result = game.getGameStatus();
        std::string winner;
        if (result == "WHITE_WINS")
        {
            ai1Wins++;
            winner = ai1.getName();
        }
        else if (result == "BLACK_WINS")
        {
            ai2Wins++;
            winner = ai2.getName();
        }
        else
        {
            draws++;
            winner = "Draw";
        }

        // Store the result in the database
        if (!db.insertGameResult(ai1.getName(), ai2.getName(), winner))
        {
            std::cerr << "Failed to insert game result into database." << std::endl;
        }
    }

    std::cout << "AI 1 Wins: " << ai1Wins << std::endl;
    std::cout << "AI 2 Wins: " << ai2Wins << std::endl;
    std::cout << "Draws: " << draws << std::endl;

    return 0;
}