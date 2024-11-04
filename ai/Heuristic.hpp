#pragma once
#include "Game.hpp"
#include <limits>

class Heuristic
{
public:
    virtual double evaluate(const hive::Game &state) const = 0;
    virtual ~Heuristic() = default;
};

class PieceCountHeuristic : public Heuristic
{
public:
    double evaluate(const hive::Game &state) const override
    {
        size_t myTilesCount = state.board.getPlayerTiles(state.currentTurn).size();
        size_t opponnentTilesCount = state.board.getPlayerTiles(state.currentTurn == 'W' ? 'B' : 'W').size();

        return static_cast<double>(myTilesCount - opponnentTilesCount);
    }
};

class WinLoseHeuristic : public Heuristic
{
public:
    double evaluate(const hive::Game &state) const override
    {
        if (state.isGameOver())
        {
            if (state.getGameStatus() == "DRAW")
            {
                return 0.0;
            }
            if (state.getGameStatus() == "WHITE_WON" && state.currentTurn == 'W')
            {
                return 1.0;
            }
            if (state.getGameStatus() == "BLACK_WON" && state.currentTurn == 'B')
            {
                return 1.0;
            }

            return -1.0;
        }
        return 0.0;
    }
};

namespace hive
{
    static constexpr double INFINITY = std::numeric_limits<double>::infinity();
    static constexpr double NEGATIVE_INFINITY = -std::numeric_limits<double>::infinity();
}