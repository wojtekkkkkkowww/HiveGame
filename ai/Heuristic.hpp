#pragma once
#include "Game.hpp"
#include <limits>

namespace hive
{
constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double negative_infinity = -std::numeric_limits<double>::infinity();

} // namespace hive

namespace hive
{

class Heuristic
{
  public:
    virtual int evaluate(const Game &state, PlayerColor player) const = 0;
    virtual ~Heuristic() = default;

  protected:
    int tileValue(PieceType tileType) const
    {
        switch (tileType)
        {
        case PieceType::Queen:
            return queen;
        case PieceType::Ant:
            return ant;
        case PieceType::Spider:
            return spider;
        case PieceType::Grasshopper:
            return grasshopper;
        case PieceType::Beetle:
            return beetle;
        }
        return 0;
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
    int evaluate(const Game &state, PlayerColor player) const override
    {
        const auto &board = state.getBoard();
        int value = 0.0;

        for (const auto &pos : board.getPlayerTiles(opposite(player)))
        {
            if (isTileBlocked(pos, state))
            {
                value += tileValue(board.getTile(pos).type);
            }
        }

        return value;
    }

  private:
    bool isTileBlocked(const hive::Position &position, const Game &state) const
    {
        const auto &board = state.getBoard();

        if (board.articulationPoints.find(position) != board.articulationPoints.end())
        {
            return true;
        }

        if (board.getTile(position).type == PieceType::Beetle)
        {
            return false;
        }

        for (const auto &dir : directions)
        {
            if (board.isEmpty(position + dir) && !board.isDirectionBlocked(position, dir, 1))
            {
                return false;
            }
        }
        return true;
    }
};

class QueenSafty : public Heuristic
{
  public:
    int evaluate(const Game &state, PlayerColor player) const override
    {
        const auto &board = state.getBoard();
        auto myQueen =
            player == PlayerColor::White ? board.whiteQueen : board.blackQueen;

        return quuenAvailableMoves(state, myQueen) - tilesAroundQueen(state, myQueen);
    }

  private:
    int tilesAroundQueen(const hive::Game &state, hive::Position &Queen) const
    {
        const auto &board = state.getBoard();
        int value = 0;
        for (auto &direction : directions)
        {
            if (!board.isEmpty(Queen + direction))
            {
                value += 1;
            }
        }
        return value;
    }

    int quuenAvailableMoves(const hive::Game &state, hive::Position &Queen) const
    {
        const auto &board = state.getBoard();
        if (Queen == invalidPosition)
        {
            return 0;
        }

        Board::PositionSet queenMoves;
        board.getQueenMoves(Queen, queenMoves);
        return static_cast<int>(queenMoves.size());
    }
};

class AttackOponentQueen : public QueenSafty
{
  public:
    int evaluate(const Game &state, PlayerColor player) const override
    {
        return -1 * QueenSafty::evaluate(state, opposite(player));
    }
};

class WinLoseHeuristic : public Heuristic
{
  public:
    int evaluate(const Game &state, PlayerColor player) const override
    {
        const auto &board = state.getBoard();
        auto oponentQueen =
            player == PlayerColor::White ? board.blackQueen : board.whiteQueen;
        auto myQueen =
            player == PlayerColor::White ? board.whiteQueen : board.blackQueen;
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
        const auto &board = state.getBoard();
        for (auto &direction : directions)
        {
            if (board.isEmpty(Queen + direction))
            {
                return false;
            }
        }
        return true;
    }
};

} // namespace hive
