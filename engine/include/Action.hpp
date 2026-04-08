#pragma once
#include "Position.hpp"
#include "Tile.hpp"
#include <iostream>
#include <sys/types.h>

namespace hive
{


enum class ActionType : uint8_t
{
    PLACE,
    MOVE,
    WAIT
};

struct Action2
{
    ActionType type = ActionType::WAIT;
    char tile_type = 'Q';
    Position position = {.x=0, .y=0};
    Position newPosition = {.x=0, .y=0};
};



/*
 * @brief The Action struct represents a single action in the game.
 * During the turn a player can place a tile, move a tile or wait.
 */
struct Action
{
    ActionType type = ActionType::WAIT;     // !!
    char tile_type = 'Q';          //!!
    Position position = {.x=0, .y=0};    //!!
    Position newPosition = {.x=0, .y=0}; //!!


public:
    static Action makeAction(const Tile &tile, Position position)
    {
        Action action;
        if (tile.placed)
        {
            action.type = ActionType::MOVE;
            action.position = tile.position;
            action.newPosition = position;
        }
        else
        {
            action.type = ActionType::PLACE;
            action.tile_type = tile.type;
            action.newPosition = position;
        }
        return action;
    }

    static Action makeWaitAction() { return Action{.type = ActionType::WAIT}; }

    static Action makeMoveAction(Position position, Position newPosition)
    {
        return Action{.type = ActionType::MOVE, .position = position, .newPosition = newPosition};
    }

    static Action makePlaceAction(Position position, char tile_type)
    {
        return Action{.type = ActionType::PLACE, .tile_type = tile_type, .newPosition = position};
    }

};

inline bool operator==(const Action &lhs, const Action &rhs)
{
    return lhs.type == rhs.type && lhs.tile_type == rhs.tile_type &&
            lhs.position == rhs.position && lhs.newPosition == rhs.newPosition;
}

inline bool operator<(const Action &lhs, const Action &rhs)
{
    if (lhs.type != rhs.type)
        return lhs.type < rhs.type;
    if (lhs.tile_type != rhs.tile_type)
        return lhs.tile_type < rhs.tile_type;
    if (lhs.position != rhs.position)
        return lhs.position < rhs.position;
    return lhs.newPosition < rhs.newPosition;
}

inline std::ostream& operator<<(std::ostream& os, const Action& action)
  {
        os << "Action: ";
        if (action.type == ActionType::PLACE)
        {
            os << "Place " << action.tile_type << " at position (" << action.newPosition.x << ", "
               << action.newPosition.y << ")";
        }
        else if (action.type == ActionType::MOVE)
        {
            os << "Move from position (" << action.position.x << ", " << action.position.y
               << ") to position (" << action.newPosition.x << ", " << action.newPosition.y << ")";
        }
        else if (action.type == ActionType::WAIT)
        {
            os << "Wait";
        }
        return os;
}


/*
to moze nie byc w sumie testowane poprawnie
*/
// struct WaitAction : public Action
// {
//     WaitAction() { makeWaitAction(); }
// };

} // namespace hive
