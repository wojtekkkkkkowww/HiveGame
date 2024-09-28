#pragma once

#include <set>
#include <utility>
#include <functional>
#include "Position.hpp"
#include "Board.hpp"


namespace hive
{
    class Board;
    using MoveFunc = std::function<std::set<Position>(Board &, Position)>;
    std::set<Position> getQueenBeeMoves(Board &board, Position position);
    std::set<Position> getBeetleMoves(Board &board, Position position);
    std::set<Position> getSpiderMoves(Board &board, Position position);
    std::set<Position> getGrasshopperMoves(Board &board, Position position);
    std::set<Position> getAntMoves(Board &board, Position position);
}