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

    protected:
        int tileValue(char tileType) const
        {
            switch (tileType)
            {
            case 'Q':
                return queen;
            case 'A':
                return ant;
            case 'S':
                return spider;
            case 'G':
                return grasshopper;
            case 'B':
                return beetle;
            default:
                return 0;
            }
        }

        static constexpr const int queen = 5;
        static constexpr const int ant = 3;
        static constexpr const int spider = 1;
        static constexpr const int grasshopper = 1;
        static constexpr const int beetle = 1;
    };

    class BlockedTiles : public Heuristic
    {
    public:
        int evaluate(const Game &state, char player) const override
        {
            int value = 0.0;
            // for (const auto &pos : state.board.getPlayerTiles(player))
            // {
            //     if (isTileBlocked(pos, state))
            //     {
            //         value -= tileValue(state.board.getTile(pos).type);
            //     }
            // }

            for (const auto &pos : state.board.getPlayerTiles(player == 'W' ? 'B' : 'W'))
            {
                if (isTileBlocked(pos, state))
                {
                    value += tileValue(state.board.getTile(pos).type);
                }
            }

            return value;
        }

    private:
        bool isTileBlocked(const hive::Position &position, const Game &state) const
        {
            
            if(state.board.articulationPoints.find(position) != state.board.articulationPoints.end())
            {
                return true;
            }
            
            if(state.board.getTile(position).type == 'B')
            {
                return false;
            }

            for(const auto &dir : directions)
            {
                if(state.board.isEmpty(position + dir) && !state.board.isDirectionBlocked(position,dir,1))
                {
                    return false;
                } 
            }
            return true;
        }
    };

    class QueenSafty : public Heuristic  // wrzucic tu jeszcze is tile blocked ?? nie no nie xD
    {
    public:
        int evaluate(const Game &state, char player) const override
        {
            auto myQueen = player == 'W' ? state.board.whiteQueen : state.board.blackQueen;

            return quuenAvailableMoves(state, myQueen) - tilesAroundQueen(state, myQueen);
        }

    private:
        int tilesAroundQueen(const hive::Game &state, hive::Position &Queen) const
        {
            int value = 0;
            for (auto &direction : directions)
            {
                if (!state.board.isEmpty(Queen + direction))
                {
                    value += 1;
                }
            }
            return value;
        }

        int quuenAvailableMoves(const hive::Game &state, hive::Position &Queen) const
        {
            if (Queen == invalidPosition)
            {
                return 0;
            }

            return static_cast<int>(state.board.getQueenMoves(Queen).size());
        }
    };

    class AttackOponentQueen : public QueenSafty
    {
    public:
        int evaluate(const Game &state, char player) const override
        {
            auto opponent = player == 'W' ? 'B' : 'W';
            return -1 * QueenSafty::evaluate(state, opponent);
        }
    };

    // class QueenAvailableMoves : public Heuristic
    // {
    // public:
    //     int evaluate(const Game &state, char player) const override
    //     {

    //         Position myQueen = player == 'W' ? state.board.whiteQueen : state.board.blackQueen;
    //         if (myQueen == invalidPosition)
    //         {
    //             return 0.0;
    //         }

    //         return static_cast<int>(state.board.getQueenMoves(myQueen).size());
    //     }
    // };

    // class OpponentQueenAvailableMoves : public QueenAvailableMoves
    // {
    // public:
    //     int evaluate(const Game &state, char player) const override
    //     {
    //         return -1 * QueenAvailableMoves::evaluate(state, player == 'W' ? 'B' : 'W');
    //     }
    // };

    // class TilesOroundOpponentQueen : public Heuristic
    // {
    // public:
    //     int evaluate(const Game &state, char player) const override
    //     {
    //         int value = 0.0;
    //         char opponent = player == 'W' ? 'B' : 'W';

    //         Position queenPosition = opponent == 'W' ? state.board.whiteQueen : state.board.blackQueen;
    //         std::set<Position> neighbours = state.board.getNeighbours(queenPosition);

    //         for (const auto &neighbour : neighbours)
    //         {
    //             if (!state.board.isEmpty(neighbour))
    //             {
    //                 value += 1.0;
    //             }
    //         }

    //         return value;
    //     }
    // };

    // class TilesOroundQuuen : public TilesOroundOpponentQueen
    // {
    // public:
    //     int evaluate(const Game &state, char player) const override
    //     {
    //         return -1 * TilesOroundOpponentQueen::evaluate(state, player);
    //     }
    // };

    class WinLoseHeuristic : public Heuristic
    {
    public:
        int evaluate(const Game &state, char player) const override
        {
            auto oponentQueen = player == 'W' ? state.board.blackQueen : state.board.whiteQueen;
            auto myQueen = player == 'W' ? state.board.whiteQueen : state.board.blackQueen;
            int value = 0;
            if (isSurrounded(state, oponentQueen))
            {
                value += 10000;
            }
            if (isSurrounded(state, myQueen))
            {
                value -= 10000;
            }

            return value;
        }

    private:
        bool isSurrounded(const hive::Game &state, hive::Position &Queen) const
        {
            for (auto &direction : directions)
            {
                if (state.board.isEmpty(Queen + direction))
                {
                    return false;
                }
            }
            return true;
        }
    };

    // class
}
