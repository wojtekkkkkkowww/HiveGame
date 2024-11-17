#include "Game.hpp"
#include "AiAlgorithm.hpp"
#include "RandomAi.hpp"
#include "AlphaBethaSearch.hpp"
#include "Heuristic.hpp"
#include "DataBase.hpp"
#include "FileOperations.hpp"
#include "Pvs.hpp"
#include <iostream>
#include <memory>
#include <fstream>
#include <chrono>
#include <cmath>
#include <random>
#include <vector>
#include <iomanip>
#include <future>

#define NUMBER_OF_GAMES 1
#define MAX_NUMBER_OF_MOVES 60

using namespace hive;

const int NUM_ITERATIONS = 20;
const int GAMES_PER_ITERATION = 2;

void perft(Game &game, int depth, long &nodes)
{
    if (depth == 0 || game.isGameOver())
    {
        nodes++;
        return;
    }

    game.genAvailableActions();
    auto actions = game.getAvailableActions();
    for (const auto &action : actions)
    {

        try
        {
            game.applyValidAction(action);
            perft(game, depth - 1, nodes);
            game.revertAction(actions);
        }
        catch (std::exception &e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
            std::cerr << "Action: " << action << std::endl;
            continue;
        }
    }
}

void performPerftTest(Game &game, int maxDepth)
{
    std::cout << "Depth & Time (ms) & Nodes & Nodes/s \\\\" << std::endl;
    for (int depth = 0; depth <= maxDepth; ++depth)
    {
        long nodes = 0;
        auto start = std::chrono::steady_clock::now();

        game.startNewGame();
        perft(game, depth, nodes);

        auto end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        double nodesPerMs = nodes / (elapsed > 0.0 ? elapsed : 1.0);
        std::cout << std::fixed << std::setprecision(2);

        std::cout << depth << " & " << elapsed << " & " << nodes << " & " << nodesPerMs << "K \\\\" << std::endl;
    }
}

void perturbWeights(std::vector<int> &weights, int maxChange)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(-maxChange, maxChange);

    for (auto &weight : weights)
    {
        weight += dist(gen);
        weight = std::max(0, weight);
        weight = std::min(20, weight);
    }
}

int playTestGame(char player, AIAlgorithm &ai1, AIAlgorithm &ai2, Game &game)
{
    game.startNewGame();
    int moves = 0;

    AIAlgorithm *currentAi = &ai1;
    AIAlgorithm *opponentAi = &ai2;

    while (!game.isGameOver() && moves < MAX_NUMBER_OF_MOVES)
    {
        Action action = currentAi->getNextMove();
        game.applyAction(action);
        moves++;
        std::swap(currentAi, opponentAi);
    }

    std::cout << "END GAME" << std::endl;
    std::string result = game.getGameStatus();

    if (result == "WHITE_WINS")
        return player == 'W' ? 1 : -1;
    else if (result == "BLACK_WINS")
        return player == 'B' ? 1 : -1;
    else
        return 0;
}

int runParallelGames(std::vector<int> &weights1, std::vector<int> &weights2)
{
    std::vector<std::future<int>> futures;

    // Launch each game simulation in a separate future
    for (int j = 0; j < GAMES_PER_ITERATION; ++j)
    {
        futures.push_back(std::async(std::launch::async, [weights1, weights2]()
                                     {
            Game game;
            AlphaBetaAI ai1(game);
            ai1.setHeuristicWeights(weights1);
            AlphaBetaAI ai2(game);
            ai2.setHeuristicWeights(weights2);
            game.startNewGame();
            int score = 0;

            // Run the game twice: once with optimized AI as white, once as black
            score += playTestGame('W', ai1, ai2, game);

            game.startNewGame();  // Reset game for the next simulation
            

            int black_score = playTestGame('B', ai2, ai1, game);
            score += black_score;

            return score; }));
    }

    // Collect all results once they are completed
    int totalScore = 0;
    for (auto &fut : futures)
    {
        totalScore += fut.get();
    }

    return totalScore;
}

int runParallelGames(std::vector<int> &weights)
{
    std::vector<std::future<int>> futures;

    // Launch each game simulation in a separate future
    for (int j = 0; j < GAMES_PER_ITERATION; ++j)
    {
        futures.push_back(std::async(std::launch::async, [weights]()
                                     {
            Game game;
            AlphaBetaAI ai1(game);
            ai1.setHeuristicWeights(weights);
            RandomAIAlgorithm ai2(game);
            game.startNewGame();
            int score = 0;

            // Run the game twice: once with optimized AI as white, once as black
            score += playTestGame('W', ai1, ai2, game);
            game.startNewGame();  // Reset game for the next simulation
            score += playTestGame('B', ai2, ai1, game);

            return score; }));
    }

    // Collect all results once they are completed
    int totalScore = 0;
    for (auto &fut : futures)
    {
        totalScore += fut.get();
    }

    return totalScore;
}

void localSearch()
{                                        //0 9 17 13
    std::vector<int> globalBestWeights = {1, 1, 1, 1, 1, 1};
    double globalBestScore = -20;

    /*
            {queenAvailableMoves, 37},
            {opponentQueenAvailableMoves, 55},
            {tilesOroundOpponentQueen, 58},
            {tilesOroundQuuen, 40},
            {blockedTiles, 58},
            {tilesValueHeuristic, 40},
    */
    std::vector<int> weights = {2, 19, 13, 12, 6, 3 };          // Initial heuristic weights
    double bestScore = -40;
    int i = 0;
    while ( i < NUM_ITERATIONS)
    {
        std::cout << "Iteration: " << i << std::endl;
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        std::vector<int> newWeights = weights;
        perturbWeights(newWeights, 5);

        std::cout << weights[0] << " " << weights[1] << " " << weights[2] << " " << weights[3] << " " << weights[4] << " " << weights[5];
        std::cout << " VS ";
        std::cout << newWeights[0] << " " << newWeights[1] << " " << newWeights[2] << " " << newWeights[3] << " " << newWeights[4] << " " << newWeights[5] << std::endl;

        double score = runParallelGames(weights, newWeights);

        std::cout << "Score " << score << std::endl;
        if (score < 0)
        {
            weights = newWeights;
            std::cout << "New weights: " << weights[0] << " " << weights[1] << " " << weights[2] << " " << weights[3] << std::endl;
            bestScore = score;
            i++;
        }else{
            i = 0;
        }

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Iteration Time = " << (end - begin).count() << std::endl;
    }

    std::cout << "Optimized Heuristic Weights: ";
    for (int weight : weights)
    {
        std::cout << weight << " ";
    }
    std::cout << std::endl;
    std::cout << "Best Score: " << bestScore << std::endl;

    for (int weight : globalBestWeights)
    {
        std::cout << weight << " ";
    }
    std::cout << std::endl;
    std::cout << "Global Best Score: " << globalBestScore << std::endl;
}

void runGameSimulation(std::unique_ptr<Game> &game, std::unique_ptr<AIAlgorithm> &ai1, std::unique_ptr<AIAlgorithm> &ai2)
{
    int ai1Wins = 0, ai2Wins = 0, draws = 0;

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
                std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
                action = ai2->getNextMove();
                std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
                std::cout << "AI 2: " << action << std::endl;
                if (ai2->getName() == "AlphaBetaAI")
                {
                    std::cout << "Nodes number: " << dynamic_cast<AlphaBetaAI *>(ai2.get())->NodesNumber << std::endl;
                    std::cout << "Time: " << (end - begin).count() << std::endl;
                }
                if (ai2->getName() == "PVS")
                {
                    std::cout << "Nodes number: " << dynamic_cast<PVS_AI *>(ai2.get())->NodesNumber << std::endl;
                    std::cout << "Time: " << (end - begin).count() << std::endl;
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
        if (result == "WHITE_WINS")
            ai1Wins++;
        else if (result == "BLACK_WINS")
            ai2Wins++;
        else
            draws++;
    }

    std::cout << "AI 1 Wins: " << ai1Wins << std::endl;
    std::cout << "AI 2 Wins: " << ai2Wins << std::endl;
    std::cout << "Draws: " << draws << std::endl;
}

void setAi(char *arg, std::unique_ptr<hive::AIAlgorithm> &ai, std::unique_ptr<hive::Game> &game)
{
    if (std::string(arg) == "random")
        ai = std::make_unique<RandomAIAlgorithm>(*game);
    else if (std::string(arg) == "alphabeta")
        ai = std::make_unique<AlphaBetaAI>(*game);
    else if (std::string(arg) == "pvs")
        ai = std::make_unique<PVS_AI>(*game);

    else
        throw std::runtime_error("Unknown AI");
}

int main(int argc, char *argv[])
{
    auto game = std::make_unique<Game>();

    if (argc == 2)
    {
        if (std::string(argv[1]) == "optimize")
            localSearch();
        if (std::string(argv[1]) == "perft")
            performPerftTest(*game, 6);
    }
    else if (argc == 3)
    {
        // Run game simulation between two AIs
        std::unique_ptr<AIAlgorithm> ai1;
        std::unique_ptr<AIAlgorithm> ai2;

        setAi(argv[1], ai1, game);
        setAi(argv[2], ai2, game);

        runGameSimulation(game, ai1, ai2);
    }
    else
    {
        std::cerr << "Usage: " << argv[0] << " <AI1> <AI2> | optimize" << std::endl;
        std::cerr << "Implemented AIs: random, alphabeta" << std::endl;
        return 1;
    }

    return 0;
}
