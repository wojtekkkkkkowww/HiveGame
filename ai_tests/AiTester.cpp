#include "AiTester.hpp"
#include "FileOperations.hpp"
#include <chrono>
#include <fstream>
#include <future>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>

/*
Depth & Time (ms) & Nodes & Nodes/s \\
2 & 0 & 36 & 36.00K \\
3 & 0 & 216 & 216.00K \\
4 & 1 & 1296 & 1296.00K \\
5 & 14 & 7776 & 555.43K \\
6 & 53 & 46656 & 880.30K \\
7 & 266 & 279408 & 1050.41K \\
8 & 1391 & 1665980 & 1197.69K \\
*/

#define NUMBER_OF_GAMES 1
#define MAX_NUMBER_OF_MOVES 100

using namespace hive;

const int NUM_ITERATIONS = 10000;
const int GAMES_PER_ITERATION = 2;

AiTester::AiTester() : game(), alphaBetaAi(game), randomAi(game), ai1Type(""), ai2Type("") {}

void AiTester::perft(int depth, long long &nodes)
{
    nodes++;

    if (depth == 0 || game.isGameOver())
    {
        return;
    }
    game.genAvailableActions();
    auto &actions = perftActionBuffers[static_cast<std::size_t>(depth)];
    actions = game.takeAvailableActions();

    int i = 0;
    for (const auto &action : actions)
    {
        if (i > 5)
        {
            break;
        }
        game.applyValidAction(action);
        perft(depth - 1, nodes);
        game.revertValidAction();
        i++;
    }
}

void AiTester::performPerftTest(int maxDepth)
{
    perftActionBuffers.resize(static_cast<std::size_t>(maxDepth + 1));
    std::cout << "Depth & Time (ms) & Nodes & Nodes/s \\\\" << '\n';
    for (int depth = 2; depth <= maxDepth; ++depth)
    {
        long long nodes = 0;
        auto start = std::chrono::steady_clock::now();

        game.startNewGame();
        perft(depth, nodes);

        auto end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        double nodesPerMs =
            elapsed > 0 ? static_cast<double>(nodes) / static_cast<double>(elapsed) : 0.0;
        std::cout << std::fixed << std::setprecision(2);

        std::cout << depth << " & " << elapsed << " & " << nodes << " & " << nodesPerMs << "K \\\\"
                  << '\n';
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

int AiTester::playTestGame(PlayerColor player, AIAlgorithm &ai1, AIAlgorithm &ai2, Game &game) const
{
    game.startNewGame();
    int moves = 0;

    AIAlgorithm *currentAi = &ai1;
    AIAlgorithm *opponentAi = &ai2;

    while (!game.isGameOver() && moves < MAX_NUMBER_OF_MOVES)
    {
        Action action = currentAi->getNextMove();
        if (!game.applyAction(action))
        {
            throw std::runtime_error("AI produced invalid action");
        }
        moves++;
        std::swap(currentAi, opponentAi);
    }

    const GameStatus result = game.getGameStatus();

    if (result == GameStatus::WhiteWins)
        return player == PlayerColor::White ? 1 : -2;
    else if (result == GameStatus::BlackWins)
        return player == PlayerColor::Black ? 1 : -2;
    else
    {
        if (result == GameStatus::Draw)
            std::cout << "DRAW" << '\n';
        return 0;
    }
}

int AiTester::runParallelGames(std::vector<double> &weights1, std::vector<double> &weights2) const
{
    std::vector<std::future<int>> futures;

    for (int j = 0; j < GAMES_PER_ITERATION; ++j)
    {
        futures.push_back(std::async(std::launch::async,
                                     [this, weights1, weights2]()
                                     {
                                         Game game;
                                         AlphaBetaAI ai1(game);
                                         ai1.setHeuristicWeights(weights1);
                                         AlphaBetaAI ai2(game);
                                         ai2.setHeuristicWeights(weights2);
                                         game.startNewGame();
                                         int score = 0;

                                         score += playTestGame(PlayerColor::White, ai1, ai2, game);
                                         return score;
                                     }));

        futures.push_back(std::async(std::launch::async,
                                     [this, weights1, weights2]()
                                     {
                                         Game game;
                                         AlphaBetaAI ai1(game);
                                         ai1.setHeuristicWeights(weights1);
                                         AlphaBetaAI ai2(game);
                                         ai2.setHeuristicWeights(weights2);
                                         game.startNewGame();
                                         int score = 0;
                                         score += playTestGame(PlayerColor::Black, ai2, ai1, game);

                                         return score;
                                     }));
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

    futures.reserve(GAMES_PER_ITERATION);
    for (int j = 0; j < GAMES_PER_ITERATION; ++j)
    {
        futures.push_back(std::async(std::launch::async,
                                     [this, weights]()
                                     {
                                         Game game;
                                         AlphaBetaAI ai1(game);
                                         ai1.setHeuristicWeights(weights);
                                         RandomAIAlgorithm ai2(game);
                                         game.startNewGame();
                                         int score = 0;

                                         score += playTestGame(PlayerColor::White, ai1, ai2, game);
                                         game.startNewGame();
                                         score += playTestGame(PlayerColor::Black, ai2, ai1, game);

                                         return score;
                                     }));
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
    std::cout << "Initial Score: " << currentScore << '\n';

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
        std::cout << "Iteration: " << i << '\n';
        std::cout << "Temperature: " << temperature << '\n';
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        std::vector<double> neighbour = neighbourWeigths(weights);
        std::cout << weights[0] << " " << weights[1] << " " << weights[2];
        std::cout << " VS ";
        std::cout << neighbour[0] << " " << neighbour[1] << " " << neighbour[2] << '\n';

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
                std::cout << "New best weights: " << weights[0] << " " << weights[1] << " "
                          << weights[2] << '\n';
            }
            scores.push_back(std::make_pair(weights, score));
        }

        temperature *= 1 - coolingRate;
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Iteration Time = " << (end - begin).count() << '\n';
        i++;
    }

    std::cout << "Optimized Heuristic Weights: ";
    for (double weight : weights)
    {
        std::cout << weight << " ";
    }
    std::cout << '\n';
    std::cout << "Current Score: " << currentScore << '\n';
    std::cout << "Best Score: " << bestScore << '\n';

    std::ofstream cacheFile("cache.txt");
    if (cacheFile.is_open())
    {
        for (const auto &entry : scores)
        {
            if (entry.second > 0)
                cacheFile << entry.first[0] << " " << entry.first[1] << " " << entry.first[2] << " "
                          << entry.second << '\n';
        }
        cacheFile.close();
    }
}

void hive::AiTester::testAI()
{
    int score = 0;
    for (int i = 0; i < 1000; i++)
    {
        game.startNewGame();
        score += playTestGame(PlayerColor::White, alphaBetaAi, randomAi, game);
        game.startNewGame();
        score += playTestGame(PlayerColor::Black, randomAi, alphaBetaAi, game);
        std::cout << "Score: " << score << '\n';
    }
    std::cout << "Total Score: " << score << '\n';
}

void AiTester::runGameSimulation()
{
    int ai1Wins = 0, ai2Wins = 0, draws = 0;

    for (int i = 0; i < NUMBER_OF_GAMES; ++i)
    {
        game.startNewGame();
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        std::vector<std::string> moves;
        int movesCounter = 0;
        for (int j = 0; j < MAX_NUMBER_OF_MOVES && !game.isGameOver(); ++j)
        {
            Action action;
            if (game.getCurrentTurn() == PlayerColor::White)
            {
                action = getNextMove(ai1Type);
                std::cout << "AI 1: " << action << '\n';
            }
            else
            {
                action = getNextMove(ai2Type);
                std::cout << "AI 2: " << action << '\n';
            }

            if (!game.applyAction(action))
            {
                throw std::runtime_error("AI produced invalid action in simulation");
            }
            moves.push_back(game.getLastAction());
            movesCounter++;
            std::cout << "Move: " << moves.back() << '\n';
        }
        std::cout << "movesCounter: " << movesCounter << '\n';
        std::cout << "number of moves: " << moves.size() << '\n';
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Game Time = " << (end - begin).count() << '\n';
        saveToFile(std::to_string(i), moves);

        const GameStatus result = game.getGameStatus();
        if (result == GameStatus::WhiteWins)
            ai1Wins++;
        else if (result == GameStatus::BlackWins)
            ai2Wins++;
        else
            draws++;
    }

    std::cout << "AI 1 Wins: " << ai1Wins << '\n';
    std::cout << "AI 2 Wins: " << ai2Wins << '\n';
    std::cout << "Draws: " << draws << '\n';
}

Action hive::AiTester::getNextMove(const std::string &aiType)
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
                std::cerr << "Game Over" << '\n';
                break;
            }

            game.genAvailableActions();
            auto actions = game.getAvailableActions();
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
        std::cout << '\n';
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
    else if (argc == 3 && std::string(argv[1]) == "perft")
    {
        tester.performPerftTest(std::stoi(argv[2]));
    }
    else if (argc == 3)
    {
        tester.setAi(argv[1], argv[2]);
        tester.runGameSimulation();
    }
    else
    {
        std::cerr << "Usage: " << argv[0] << " <AI1> <AI2> | optimize" << '\n';
        std::cerr << "Implemented AIs: random, alphabeta" << '\n';
        return 1;
    }

    return 0;
}
