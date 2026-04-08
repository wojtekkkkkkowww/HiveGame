#pragma once

#include "MoveValidator.hpp"
#include "Position.hpp"
#include <map>
#include <set>

namespace hive
{
/*
 * @brief The MovementManager class is responsible for generating possible moves for each tile type.
 */
class MovementManager : public MoveValidator
{
  public:
    MovementManager();

    // API: output parameters for better performance
    void getAvailableMoves(char type, const Position &position, std::set<Position> &outMoves);
    void getAntMoves(const Position &position, std::set<Position> &outMoves);
    void getSpiderMoves(const Position &position, std::set<Position> &outMoves);
    void getGrasshopperMoves(const Position &position, std::set<Position> &outMoves) const;
    void getBeetleMoves(const Position &position, std::set<Position> &outMoves) const;
    void getQueenMoves(const Position &position, std::set<Position> &outMoves) const;

    // Legacy cache (for backwards compatibility)
    std::map<Position, std::set<Position>> antMoves;
};

} // namespace hive