#pragma once
#include <format>
#include <variant>
#include <string>
#include <initializer_list>
#include <stdexcept>
#include <iostream>
#include "Tile.hpp"
#include "Position.hpp"

namespace hive {

struct Action {
    std::string type;
    std::string tile_type = "QUEEN";
    Position position;
    Position newPosition = {0, 0};

    Action(std::initializer_list<std::variant<std::string, Position>> init_list) {
        auto it = init_list.begin();
        type = std::get<std::string>(*it++);
        position = std::get<Position>(*it++);
        if (type == "PLACE") {
            tile_type = std::get<std::string>(*it);
        } else if (type == "MOVE") {
            newPosition = std::get<Position>(*it);
        } else if (type == "WAIT") {
            makeWaitAction();
        } else {
            throw std::invalid_argument("Invalid action type");
        }
    }

    void makeWaitAction() {
        type = "WAIT";
    }

    void makeMoveAction(Position position, Position newPosition) {
        this->type = "MOVE";
        this->position = position;
        this->newPosition = newPosition;
    }

    void makePlaceAction(Position position, std::string tile_type) {
        this->type = "PLACE";
        this->tile_type = tile_type;
        this->position = position;
    }

    friend bool operator==(const Action &lhs, const Action &rhs) {
        return lhs.type == rhs.type &&
               lhs.tile_type == rhs.tile_type &&
               lhs.position == rhs.position &&
               lhs.newPosition == rhs.newPosition;
    }

    friend bool operator<(const Action &lhs, const Action &rhs) {
        if (lhs.type != rhs.type)
            return lhs.type < rhs.type;
        if (lhs.tile_type != rhs.tile_type)
            return lhs.tile_type < rhs.tile_type;
        if (lhs.position != rhs.position)
            return lhs.position < rhs.position;
        return lhs.newPosition < rhs.newPosition;
    }

    friend std::ostream &operator<<(std::ostream &os, const Action &action) {
        os << "Action: ";
        if (action.type == "PLACE") {
            os << "Place " << action.tile_type << " at position (" << action.position.x << ", " << action.position.y << ")";
        } else if (action.type == "MOVE") {
            os << "Move from position (" << action.position.x << ", " << action.position.y << ") to position (" << action.newPosition.x << ", " << action.newPosition.y << ")";
        } else if (action.type == "WAIT") {
            os << "Wait";
        }
        return os;
    }
};

}