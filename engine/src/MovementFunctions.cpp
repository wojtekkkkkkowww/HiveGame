#include "MovementFunctions.hpp"
#include "Board.hpp"

namespace hive
{

    std::set<std::pair<int, int>> getQueenBeeMoves(Board &board, std::pair<int, int> position)
    {
        std::set<std::pair<int, int>> neighbours = board.getNeighbours(position);
        std::set<std::pair<int, int>> moves;

        for (auto &neighbour : neighbours)
        {
            if (board.isEmpty(neighbour))
            {
                moves.insert(neighbour);
            }
        }
        return moves;
    }

    std::set<std::pair<int, int>> getBeetleMoves(Board &board, std::pair<int, int> position)
    {
        return std::set<std::pair<int, int>>();
    }
    std::set<std::pair<int, int>> getSpiderMoves(Board &board, std::pair<int, int> position)
    {
        return std::set<std::pair<int, int>>();
    }
    std::set<std::pair<int, int>> getGrasshopperMoves(Board &board, std::pair<int, int> position)
    {
        return std::set<std::pair<int, int>>();
    }
    std::set<std::pair<int, int>> getAntMoves(Board &board, std::pair<int, int> position)
    {
        return std::set<std::pair<int, int>>();
    }
}