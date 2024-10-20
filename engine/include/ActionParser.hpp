#pragma once
#include "Action.hpp"
#include "Position.hpp"
#include <sstream>

namespace hive
{

    class ActionParser
    {
    public:
        static Action stringToAction(const std::string &actionStr)
        {

            std::vector<std::string> tokens = splitString(actionStr, '|');

            if (tokens[0] == "P")
            {
                if (tokens.size() < 4)
                    throw std::invalid_argument("Invalid place action string");
                char tile_type = tokens[1][0];
                int x = std::stoi(tokens[2]);
                int y = std::stoi(tokens[3]);
                return PlaceAction({x, y}, tile_type);
            }
            else if (tokens[0] == "M")
            {
                if (tokens.size() < 5)
                    throw std::invalid_argument("Invalid move action string");
                int x = std::stoi(tokens[1]);
                int y = std::stoi(tokens[2]);
                int newX = std::stoi(tokens[3]);
                int newY = std::stoi(tokens[4]);
                return MoveAction({x, y}, {newX, newY});
            }
            else if (tokens[0] == "W")
            {
                return WaitAction();
            }

            throw std::invalid_argument("Invalid action string");
        }

        static std::string actionToString(const Action &action)
        {
            std::ostringstream oss;
            if (action.type == "PLACE")
            {
                oss << "P|" << action.tile_type << "|" << action.position.x << "|" << action.position.y << "|";
            }
            else if (action.type == "MOVE")
            {
                oss << "M|" << action.position.x << "|" << action.position.y << "|" << action.newPosition.x << "|" << action.newPosition.y << "|";
            }
            else if (action.type == "WAIT")
            {
                oss << "W|";
            }
            return oss.str();
        }

    private:
        static std::vector<std::string> splitString(const std::string &str, char delimiter)
        {
            std::vector<std::string> tokens;
            std::stringstream ss(str);
            std::string token;

            while (std::getline(ss, token, delimiter))
            {
                if (!token.empty())
                {
                    tokens.push_back(token);
                }
            }

            return tokens;
        }
    };
}