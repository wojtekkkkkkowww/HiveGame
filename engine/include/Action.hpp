#pragma once
#include <format>
#include <variant>
#include <string>
#include <initializer_list>
#include <stdexcept>
#include <iostream>
#include "Tile.hpp"
#include "Position.hpp"

namespace hive
{
    /*
     * @brief The Action struct represents a single action in the game.
     * During the turn a player can place a tile, move a tile or wait.
     */
    struct Action
    {
        std::string type = "WAIT";     // !!
        char tile_type = 'Q';          //!!
        Position position = {0, 0};    //!!
        Position newPosition = {0, 0}; //!!

        Action() {}

        Action(Tile tile, Position position)
        {
            makeAction(tile, position);
        }

        void makeAction(Tile tile, Position position)
        {
            if (tile.placed)
            {
                this->type = "MOVE";
                this->position = tile.position;
                this->newPosition = position;
            }
            else
            {
                this->type = "PLACE";
                this->tile_type = tile.type;
                this->newPosition = position;
            }
        }

        void makeWaitAction()
        {
            this->type = "WAIT";
        }

        void makeMoveAction(Position position, Position newPosition)
        {
            this->type = "MOVE";
            this->position = position;
            this->newPosition = newPosition;
        }

        void makePlaceAction(Position position, char tile_type)
        {
            this->type = "PLACE";
            this->tile_type = tile_type;
            this->newPosition = position;
        }

        friend bool operator==(const Action &lhs, const Action &rhs)
        {
            return lhs.type == rhs.type &&
                   lhs.tile_type == rhs.tile_type &&
                   lhs.position == rhs.position &&
                   lhs.newPosition == rhs.newPosition;
        }

        friend bool operator<(const Action &lhs, const Action &rhs)
        {
            if (lhs.type != rhs.type)
                return lhs.type < rhs.type;
            if (lhs.tile_type != rhs.tile_type)
                return lhs.tile_type < rhs.tile_type;
            if (lhs.position != rhs.position)
                return lhs.position < rhs.position;
            return lhs.newPosition < rhs.newPosition;
        }

        friend std::ostream &operator<<(std::ostream &os, const Action &action)
        {
            os << "Action: ";
            if (action.type == "PLACE")
            {
                os << "Place " << action.tile_type << " at position (" << action.newPosition.x << ", " << action.newPosition.y << ")";
            }
            else if (action.type == "MOVE")
            {
                os << "Move from position (" << action.position.x << ", " << action.position.y << ") to position (" << action.newPosition.x << ", " << action.newPosition.y << ")";
            }
            else if (action.type == "WAIT")
            {
                os << "Wait";
            }
            return os;
        }
    };

    struct MoveAction : public Action
    {
        MoveAction(Position position, Position newPosition)
        {
            makeMoveAction(position, newPosition);
        }
    };

    struct PlaceAction : public Action
    {
        PlaceAction(Position position, char tile_type)
        {
            makePlaceAction(position, tile_type);
        }
    };

    struct WaitAction : public Action
    {
        WaitAction()
        {
            makeWaitAction();
        }
    };

}
