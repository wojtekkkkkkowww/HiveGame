#pragma once
#include "Game.hpp"
#include <limits>

namespace hive
{

    class Heuristic
    {
    public:
        virtual double evaluate(const Game &state, char player) const = 0;  // Include char player
        virtual ~Heuristic() = default;
    };

    class PlacingQueenHeuristic : public Heuristic
    {
        double evaluate(const Game &state, char player) const override
        {
            if (player == 'W')
            {
                return state.board.whiteQueen == invalidPosition ? 1.0 : 0.0;
            }
            else
            {
                return state.board.blackQueen == invalidPosition ? 1.0 : 0.0;
            }
        }
    };


    class PieceCountHeuristic : public Heuristic
    {
    public:
        double evaluate(const Game &state, char player) const override
        {
            size_t myTilesCount = state.board.getPlayerTiles(player).size();
            
            return static_cast<double>(myTilesCount);
        }
    };

    class QueenAvailableMoves : public Heuristic
    {
    public:
        double evaluate(const Game &state, char player) const override
        {

            Position myQueen = player == 'W' ? state.board.whiteQueen : state.board.blackQueen;
            if (myQueen == invalidPosition)
            {
                return 0.0;
            }

            return static_cast<double>(state.board.getAvailableMoves('Q',myQueen).size());
        }
    };

    class WinLoseHeuristic : public Heuristic
    {
    public:
        double evaluate(const Game &state, char player) const override
        {
            auto oponentQueen = player == 'W' ? state.board.blackQueen : state.board.whiteQueen;    
            for(auto &direction : directions)
            {
                if(state.board.isEmpty(oponentQueen + direction))
                {
                    return 0.0;
                }
            }
            return 1.0;
        }
    };

    class TilesOroundOpponentQueen : public Heuristic
    {
    public:
        double evaluate(const Game &state, char player) const override
        {
            double value = 0.0;
            char opponent = player == 'W' ? 'B' : 'W';

            Position queenPosition = opponent == 'W' ? state.board.whiteQueen : state.board.blackQueen;
            std::set<Position> neighbours = state.board.getNeighbours(queenPosition);

            for (const auto &neighbour : neighbours)
            {
                if (!state.board.isEmpty(neighbour))
                {
                    value += 1.0;
                }
            }

            return value;
        }
    };
}

namespace hive
{
    static constexpr double INFINITY = std::numeric_limits<double>::infinity();
    static constexpr double NEGATIVE_INFINITY = -std::numeric_limits<double>::infinity();
}