#include "AiTester.hpp"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <random>

#define NUMBER_OF_GAMES 1
#define MAX_NUMBER_OF_MOVES 60

using namespace hive;

const int NUM_ITERATIONS = 20;
const int GAMES_PER_ITERATION = 20;

AiTester::AiTester()
    : game(),
      alphaBetaAi(game), randomAi(game),
      pvsAi(game), ai1Type(""), ai2Type("") {}

void AiTester::perft(int depth, long long &nodes)
{
    nodes++;

    if (depth == 0 || game.isGameOver())
    {
        return;
    }
    game.genAvailableActions();
    auto actions = game.avaliableActions;
    
    auto emptyTiles = game.board.emptyTiles;

    int i = 0;
    for (const auto &action : actions)
    {
        // if(i > 5){
        //    break;
        // }
        game.applyValidAction(action);
        perft(depth - 1, nodes);
        game.revertAction(actions, emptyTiles);
        i++;
    }
}

void AiTester::performPerftTest(int maxDepth)
{
    std::cout << "Depth & Time (ms) & Nodes & Nodes/s \\\\" << std::endl;
    for (int depth = 2; depth <= maxDepth; ++depth)
    {
        long long nodes = 0;
        auto start = std::chrono::steady_clock::now();

        game.startNewGame();
        perft(depth, nodes);

        auto end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        double nodesPerMs = nodes / (elapsed > 0.0 ? elapsed : 1.0);
        std::cout << std::fixed << std::setprecision(2);

        std::cout << depth << " & " << elapsed << " & " << nodes << " & " << nodesPerMs << "K \\\\" << std::endl;
    }
}

void AiTester::perturbWeights(std::vector<int> &weights, int maxChange)
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

int AiTester::playTestGame(char player, AIAlgorithm &ai1, AIAlgorithm &ai2, Game &game) const
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

int AiTester::runParallelGames(std::vector<int> &weights1, std::vector<int> &weights2) const
{
    std::vector<std::future<int>> futures;

    for (int j = 0; j < GAMES_PER_ITERATION; ++j)
    {
        futures.push_back(std::async(std::launch::async, [this, weights1, weights2]()
                                     {
            Game game;
            AlphaBetaAI ai1(game);
            ai1.setHeuristicWeights(weights1);
            AlphaBetaAI ai2(game);
            ai2.setHeuristicWeights(weights2);
            game.startNewGame();
            int score = 0;

            score += playTestGame('W', ai1, ai2, game);

            game.startNewGame();
            score += playTestGame('B', ai2, ai1, game);

            return score; }));
    }

    int totalScore = 0;
    for (auto &fut : futures)
    {
        totalScore += fut.get();
    }

    return totalScore;
}

int AiTester::runParallelGames(std::vector<int> &weights)
{
    std::vector<std::future<int>> futures;

    for (int j = 0; j < GAMES_PER_ITERATION; ++j)
    {
        futures.push_back(std::async(std::launch::async, [this, weights]()
                                     {
            Game game;
            AlphaBetaAI ai1(game);
            ai1.setHeuristicWeights(weights);
            RandomAIAlgorithm ai2(game);
            game.startNewGame();
            int score = 0;

            score += playTestGame('W', ai1, ai2, game);
            game.startNewGame();
            score += playTestGame('B', ai2, ai1, game);

            return score; }));
    }

    int totalScore = 0;
    for (auto &fut : futures)
    {
        totalScore += fut.get();
    }

    return totalScore;
}

void AiTester::localSearch()
{
    std::vector<int> weights = {1, 1, 1, 1};
    double bestScore = -40;
    int i = 0;
    while (i < NUM_ITERATIONS)
    {
        std::cout << "Iteration: " << i << std::endl;
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        std::vector<int> newWeights = weights;
        perturbWeights(newWeights, 5);

        std::cout << weights[0] << " " << weights[1] << " " << weights[2] << " " << weights[3];
        std::cout << " VS ";
        std::cout << newWeights[0] << " " << newWeights[1] << " " << newWeights[2] << " " << newWeights[3] << std::endl;

        int score = runParallelGames(newWeights);

        std::cout << "Score " << score << std::endl;
        if (score > bestScore)
        {
            weights = newWeights;
            std::cout << "New weights: " << weights[0] << " " << weights[1] << " " << weights[2] << " " << weights[3] << std::endl;
            bestScore = score;
            i = 0;
        }
        else
        {
            i++;
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
}

void AiTester::runGameSimulation()
{
    int ai1Wins = 0, ai2Wins = 0, draws = 0;

    for (int i = 0; i < NUMBER_OF_GAMES; ++i)
    {
        game.startNewGame();
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        std::vector<std::string> moves;

        for (int j = 0; j < MAX_NUMBER_OF_MOVES && !game.isGameOver(); ++j)
        {
            Action action;
            if (game.getCurrentTurn() == 'W')
            {
                action = getNextMove(ai1Type);
                std::cout << "AI 1: " << action << std::endl;
            }
            else
            {
                action = getNextMove(ai2Type);
                std::cout << "AI 2: " << action << std::endl;
            }

            game.applyAction(action);
            moves.push_back(game.getLastAction());
            std::cout << "Move: " << moves.back() << std::endl;
        }
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Game Time = " << (end - begin).count() << std::endl;
        saveToFile(std::to_string(i), moves);

        std::string result = game.getGameStatus();
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

Action hive::AiTester::getNextMove(std::string aiType)
{
    Action action;
    if (aiType == "random")
        action = randomAi.getNextMove();
    else if (aiType == "alphabeta")
        action = alphaBetaAi.getNextMove();
    else if (aiType == "pvs")
        action = pvsAi.getNextMove();

    return action;
}

void AiTester::setAi(const std::string &arg1, const std::string &arg2)
{
    ai1Type = arg1;
    ai2Type = arg2;
}

int main(int argc, char *argv[])
{
    AiTester tester;

    if (argc == 2)
    {
        if (std::string(argv[1]) == "optimize")
            tester.localSearch();
        if (std::string(argv[1]) == "perft")
            tester.performPerftTest(10);
    }
    else if (argc == 3)
    {
        tester.setAi(argv[1], argv[2]);
        tester.runGameSimulation();
    }
    else
    {
        std::cerr << "Usage: " << argv[0] << " <AI1> <AI2> | optimize" << std::endl;
        std::cerr << "Implemented AIs: random, alphabeta, pvs" << std::endl;
        return 1;
    }

    return 0;
}
