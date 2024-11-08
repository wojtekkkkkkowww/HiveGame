#pragma once

#include <set>
#include <utility>
#include <functional>
#include "Position.hpp"
#include "MoveValidator.hpp"
#include "dfs.hpp"
#include "Tiles/Beetle.hpp"
#include "Tiles/Ant.hpp"
#include "Tiles/Spider.hpp"
#include "Tiles/Queen.hpp"
#include "Tiles/Grasshopper.hpp"


namespace hive
{
    /*
    * @brief The MovementManager class is responsible for generating possible moves for each tile type.
    */
    class MovementManager : public MoveValidator
    {
    public:
        MovementManager();
        std::set<Position> getAvailableMoves(char type, const Position &position) const;
    private:
    };

}