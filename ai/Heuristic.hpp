#pragma once
#include "Game.hpp"
#include <limits>

namespace hive
{
    static constexpr int infinity = std::numeric_limits<int>::max();
    static constexpr int negative_infinity = std::numeric_limits<int>::min();

}

namespace hive
{

    class Heuristic
    {
    public:
        virtual int evaluate(const Game &state, char player) const = 0; // Include char player
        virtual ~Heuristic() = default;
    };

    class TilesValueHeuristic : public Heuristic
    {
    public:
        static const int queen = 5;
        static const int ant = 3;
        static const int spider = 1;
        static const int grasshopper = 1;
        static const int beetle = 1;

        int evaluate(const Game &state, char player) const override
        {
            int value = 0.0;
            for (const auto &pos : state.board.getPlayerTiles(player))
            {
                auto tile = state.board.getTile(pos);
                switch (tile.type)
                {
                case 'Q':
                    value += queen;
                    break;
                case 'A':
                    value += ant;
                    break;
                case 'S':
                    value += spider;
                    break;
                case 'G':
                    value += grasshopper;
                    break;
                case 'B':
                    value += beetle;
                    break;
                default:
                    break;
                }
            }
            return value;
        }
    };


    class PieceCountHeuristic : public Heuristic
    {
    public:
        int evaluate(const Game &state, char player) const override
        {
            size_t myTilesCount = state.board.getPlayerTiles(player).size();

            return static_cast<int>(myTilesCount);
        }
    };

    class QueenAvailableMoves : public Heuristic
    {
    public:
        int evaluate(const Game &state, char player) const override
        {

            Position myQueen = player == 'W' ? state.board.whiteQueen : state.board.blackQueen;
            if (myQueen == invalidPosition)
            {
                return 0.0;
            }

            return static_cast<int>(state.board.getAvailableMoves('Q', myQueen).size());
        }
    };

    class WinHeuristic : public Heuristic
    {
    public:
        int evaluate(const Game &state, char player) const override
        {
            auto oponentQueen = player == 'W' ? state.board.blackQueen : state.board.whiteQueen;
            for (auto &direction : directions)
            {
                if (state.board.isEmpty(oponentQueen + direction))
                {
                    return 0;
                }
            }
            return 10000;
        }
    };

    class LoseHeuristic : public Heuristic
    {
    public:
        int evaluate(const Game &state, char player) const override
        {
            auto myQueen = player == 'W' ? state.board.whiteQueen : state.board.blackQueen;
            for (auto &direction : directions)
            {
                if (state.board.isEmpty(myQueen + direction))
                {
                    return 0;
                }
            }
            return -10000;
        }
    };

    class TilesOroundOpponentQueen : public Heuristic
    {
    public:
        int evaluate(const Game &state, char player) const override
        {
            int value = 0.0;
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
