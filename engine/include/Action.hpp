#pragma once
#include "Position.hpp"
#include "Tile.hpp"
#include <iostream>

namespace hive
{

enum class ActionType : uint8_t
{
    PLACE,
    MOVE,
    WAIT
};

/*
 * @brief The Action struct represents a single action in the game.
 * During the turn a player can place a tile, move a tile or wait.
 */
struct Action
{
    ActionType type{};
    PieceType pieceType = PieceType::Queen;
    Position position{};
    Position newPosition{};

    auto operator<=>(const Action &) const = default;
};

constexpr Action makeWaitAction() { return Action{.type = ActionType::WAIT}; }

constexpr Action makeMoveAction(Position position, Position newPosition)
{
    return Action{.type = ActionType::MOVE, .position = position, .newPosition = newPosition};
}

constexpr Action makePlaceAction(Position position, PieceType pieceType)
{
    return Action{.type = ActionType::PLACE, .pieceType = pieceType, .newPosition = position};
}

constexpr Action makeAction(const Tile &tile, Position position)
{
    if (tile.placed)
    {
        return makeMoveAction(tile.position, position);
    }
    else
    {
        return makePlaceAction(position, tile.type);
    }
}

inline std::ostream &operator<<(std::ostream &os, const Action &action)
{
    os << "Action: ";
    if (action.type == ActionType::PLACE)
    {
        os << "Place " << toChar(action.pieceType) << " at position (" << action.newPosition.x
           << ", " << action.newPosition.y << ")";
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
} // namespace hive
