#include "AiTester.hpp"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <random>
#include <fstream>
#include <map>

#define NUMBER_OF_GAMES 1
#define MAX_NUMBER_OF_MOVES 200

using namespace hive;

const int NUM_ITERATIONS = 10000;
const int GAMES_PER_ITERATION = 2;

AiTester::AiTester()
    : game(),
      alphaBetaAi(game), randomAi(game),
      ai1Type(""), ai2Type("") {}

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
        if (i > 5)
        {
            break;
        }
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

void AiTester::randomWeights(std::vector<double> &weights)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0, 10);

    for (auto &weight : weights)
    {
        weight = dist(gen);
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

    //   std::cout << "END GAME" << std::endl;
    std::string result = game.getGameStatus();

    if (result == "WHITE_WINS")
        return player == 'W' ? 1 : -2;
    else if (result == "BLACK_WINS")
        return player == 'B' ? 1 : -2;
    else{
        if(result == "DRAW")
            std::cout << "DRAW" << std::endl;
        return 0;
    }
}

int AiTester::runParallelGames(std::vector<double> &weights1, std::vector<double> &weights2) const
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
            return score; }));

        futures.push_back(std::async(std::launch::async, [this, weights1, weights2]()
                                     {
            Game game;
            AlphaBetaAI ai1(game);
            ai1.setHeuristicWeights(weights1);
            AlphaBetaAI ai2(game);
            ai2.setHeuristicWeights(weights2);
            game.startNewGame();
            int score = 0;
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

int AiTester::runParallelGames(std::vector<double> &weights)
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



std::vector<double> AiTester::neighbourWeigths(std::vector<double> &weigths)
{
    std::vector<double> newWeights = weigths;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution dis(0, 3);
    int index = dis(gen) % 3;
    std::uniform_real_distribution<> dis2(-2, 2);
    newWeights[index] = std::min(std::max(0.0, newWeights[index] + dis2(gen)), 10.0);
    return newWeights;
}

void AiTester::SA()
{
    std::vector<double> weights = {5, 5, 5};
    std::list<std::pair<std::vector<double>, int>> scores;
    double bestScore = -40;
    double currentScore = runParallelGames(weights);
    std::cout << "Initial Score: " << currentScore << std::endl;

    std::vector<std::vector<double>> opponents;
    for (int i = 0; i < 20; i++)
    {
        std::vector<double> random = weights;
        randomWeights(random);
        opponents.push_back(random);
    }

    double temperature = 10000.0;
    double coolingRate = 0.003;
    int i = 0;

    while (i < NUM_ITERATIONS && temperature > 1)
    {
        std::cout << "Iteration: " << i << std::endl;
        std::cout << "Temperature: " << temperature << std::endl;
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        std::vector<double> neighbour = neighbourWeigths(weights);
        std::cout << weights[0] << " " << weights[1] << " " << weights[2];
        std::cout << " VS ";
        std::cout << neighbour[0] << " " << neighbour[1] << " " << neighbour[2] << std::endl;

        int score = 0;
        for (int j = 0; j < 20; j++)
        {
            score += runParallelGames(neighbour, opponents[j]);
        }

        double acceptanceProbability = exp((score - currentScore) / temperature);
        if (score > currentScore || (rand() / double(RAND_MAX)) < acceptanceProbability)
        {
            weights = neighbour;
            currentScore = score;
            if (score > bestScore)
            {
                bestScore = score;
                std::cout << "New best weights: " << weights[0] << " " << weights[1] << " " << weights[2] << std::endl;
            }
            scores.push_back(std::make_pair(weights, score));
        }

        temperature *= 1 - coolingRate;
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Iteration Time = " << (end - begin).count() << std::endl;
        i++;
    }

    std::cout << "Optimized Heuristic Weights: ";
    for (double weight : weights)
    {
        std::cout << weight << " ";
    }
    std::cout << std::endl;
    std::cout << "Current Score: " << currentScore << std::endl;
    std::cout << "Best Score: " << bestScore << std::endl;

    std::ofstream cacheFile("cache.txt");
    if (cacheFile.is_open())
    {
        for (const auto &entry : scores)
        {
            if (entry.second > 0)
                cacheFile << entry.first[0] << " "
                          << entry.first[1] << " "
                          << entry.first[2] << " "
                          << entry.second << std::endl;
        }
        cacheFile.close();
    }
}

void hive::AiTester::testAI()
{   int score = 0;
    for(int i = 0; i < 1000; i++)
    {
        game.startNewGame();
        score += playTestGame('W', alphaBetaAi, randomAi, game);
        game.startNewGame();
        score += playTestGame('B', randomAi, alphaBetaAi, game);
        std::cout << "Score: " << score << std::endl;
    }
    std::cout << "Total Score: " << score << std::endl;
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

    return action;
}

void AiTester::setAi(const std::string &arg1, const std::string &arg2)
{
    ai1Type = arg1;
    ai2Type = arg2;
}

void AiTester::performRandomMoves(int iterations, int epochs)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<std::vector<int>> branchingFactorSum(epochs, std::vector<int>(iterations, 0));
    for (int epoch = 0; epoch < epochs; ++epoch)
    {
        game.startNewGame();
        for (int i = 0; i < iterations; ++i)
        {
            if (game.isGameOver())
            {
                std::cerr << "Game Over" << std::endl;
                break;
            }

            game.genAvailableActions();
            auto actions = game.avaliableActions;
            branchingFactorSum[epoch][i] = actions.size();

            if (actions.empty())
                break;

            std::uniform_int_distribution<> dis(0, actions.size() - 1);
            auto it = std::next(actions.begin(), dis(gen));
            game.applyValidAction(*it);
        }
    }

    for (int i = 0; i < iterations; ++i)
    {
        for (int j = 0; j < epochs; ++j)
        {
            std::cout << branchingFactorSum[j][i] << " ";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char *argv[])
{
    AiTester tester;
    if (argc == 2)
    {
        if (std::string(argv[1]) == "optimize")
            tester.SA();
        if (std::string(argv[1]) == "perft")
            tester.performPerftTest(10);
        if (std::string(argv[1]) == "bf")
            tester.performRandomMoves(100, 30);
        if (std::string(argv[1]) == "test")
            tester.testAI();
    }
    else if (argc == 3)
    {
        tester.setAi(argv[1], argv[2]);
        tester.runGameSimulation();
    }
    else
    {
        std::cerr << "Usage: " << argv[0] << " <AI1> <AI2> | optimize" << std::endl;
        std::cerr << "Implemented AIs: random, alphabeta" << std::endl;
        return 1;
    }

    return 0;
}
