#pragma once

#include <set>
#include <utility>
#include <functional>

namespace hive
{
    class Board;
    using MoveFunc = std::function<std::set<std::pair<int, int>>(Board &, std::pair<int, int>)>;
    std::set<std::pair<int, int>> getQueenBeeMoves(Board &board, std::pair<int, int> position);
    std::set<std::pair<int, int>> getBeetleMoves(Board &board, std::pair<int, int> position);
    std::set<std::pair<int, int>> getSpiderMoves(Board &board, std::pair<int, int> position);
    std::set<std::pair<int, int>> getGrasshopperMoves(Board &board, std::pair<int, int> position);
    std::set<std::pair<int, int>> getAntMoves(Board &board, std::pair<int, int> position);
}