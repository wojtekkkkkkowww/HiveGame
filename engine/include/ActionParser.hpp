#pragma once
#include "Action.hpp"
#include "Tile.hpp"
#include "BaseBoard.hpp"
#include "Position.hpp"
#include "Player.hpp"
#include <sstream>
#include <regex>

namespace hive
{
    /**
     * @brief The ActionParser class is responsible for converting between string notations and Action objects.
     */
    class ActionParser
    {
    public:
        ActionParser(const BaseBoard &board, const char &currentTurn, std::stack<Action> &actions) : board(board), currentTurn(currentTurn), actions(actions) 
        {
            reset();
        }

        /**
         * @brief Converts a notation string to an Action object.
         * @param notation The string notation, e.g., "WG2-bQ".
         * @return The corresponding Action object.
         */
        Action stringToAction(const std::string &notation) const
        {
            std::regex rightDirectionRegex(R"(([WB][A-Z]\d?) ([WB][A-Z]\d?)([-\\/]))");
            std::regex beetleMoveRegex(R"(([WB][A-Z]\d?) ([WB][A-Z]\d?)\.)");
            std::regex leftDirectionRegex(R"(([WB][A-Z]\d?) ([-\\/])([WB][A-Z]\d?))");
            std::regex firstActionRegex(R"([WB][A-Z]\d?)");
            std::smatch matches;

            if (std::regex_search(notation, matches, leftDirectionRegex))
            {
                return parseLeftDirection(matches);
            }
            else if (std::regex_search(notation, matches, rightDirectionRegex))
            {
                return parseRightDirection(matches);
            }
            else if (std::regex_search(notation, matches, beetleMoveRegex))
            {
                return parseBeetleMove(matches);
            }
            else if (std::regex_search(notation, matches, firstActionRegex))
            {
                return parseFirstAction(matches);
            }

            return WaitAction{};
        }

        /**
         * @brief Converts an Action object to the corresponding string notation.
         * @param action The Action object to convert.
         * @return The string notation, e.g., "WG2-bQ".
         */
        std::string actionToString(const Action &action)
        {
            if (action.type == "WAIT")
            {
                return "Wait";
            }

            std::string tileNotation;
            if (action.type == "MOVE")
            {
                tileNotation = getMoveTileNotation(action);
            }
            else if (action.type == "PLACE")
            {
                tileNotation = getPlaceTileNotation(action);
            }

            if (board.getLevel(action.newPosition) >= 1)
            {
                Tile tile = board.getTile(action.newPosition);
                return tileNotation + " " + tile.notation + ".";
            }

            std::string neighborNotation = getNeighborNotation(action.newPosition);
            return buildActionString(tileNotation, neighborNotation, action.newPosition);
        }

        void reset()
        {
            firstMove = true;
            tileNumber = {
                {"WQ", 1},
                {"WS", 1},
                {"WB", 1},
                {"WG", 1},
                {"WA", 1},
                {"BQ", 1},
                {"BS", 1},
                {"BB", 1},
                {"BG", 1},
                {"BA", 1}};
        }

        void revert()
        {
            Action lastAction = actions.top();
            if(lastAction.type == "PLACE")
            {
                std::ostringstream os;
                os << currentTurn << lastAction.tile_type;
                tileNumber[os.str()]--;
            }
        }

    private:
        bool firstMove = true;
        std::map<std::string, int> tileNumber;
        const BaseBoard &board;
        const char &currentTurn;
        const std::stack<Action>& actions;


        Action parseLeftDirection(const std::smatch &matches) const
        {
            std::string tileNotation = matches[1].str();
            std::string direction = matches[2].str();
            std::string destinationNotation = matches[3].str();

            return createAction(tileNotation, destinationNotation, direction, "left");
        }

        Action parseRightDirection(const std::smatch &matches) const
        {
            std::string tileNotation = matches[1].str();
            std::string destinationNotation = matches[2].str();
            std::string direction = matches[3].str();

            return createAction(tileNotation, destinationNotation, direction, "right");
        }

        Action parseFirstAction(const std::smatch &matches) const
        {
            std::string tileNotation = matches[0].str();

            return PlaceAction({0, 0}, tileNotation[1]);
        }

        Action parseBeetleMove(const std::smatch &matches) const
        {
            std::string tileNotation = matches[1].str();
            std::string destinationNotation = matches[2].str();

            Tile tile = board.getTileByNotation(tileNotation);
            Tile destinationTile = board.getTileByNotation(destinationNotation);
            Position position = destinationTile.position;

            return MoveAction(tile.position, position);
        }

        std::string getMoveTileNotation(const Action &action) const
        {
            Tile tile = board.getTile(action.position);
            return tile.notation;
        }

        std::string getPlaceTileNotation(const Action &action)
        {
            std::string tileNotation = getTileNotation(action.tile_type);
            updateTileNumber(action);

            if (currentTurn == 'W' && firstMove)
            {
                firstMove = false;

                return tileNotation;
            }

            return tileNotation;
        }

        void updateTileNumber(const hive::Action &action)
        {
            std::ostringstream os;
            os << currentTurn << action.tile_type; // content of
            tileNumber[os.str()]++;
        }

        std::string getNeighborNotation(const Position &newPosition) const
        {
            std::string neighborNotation;
            Position direction = getDirection(newPosition);

            for (const auto &neighbor : board.getNeighbours(newPosition))
            {
                if (!board.isEmpty(neighbor))
                {
                    Tile n = board.getTile(neighbor);
                    neighborNotation = n.notation;
                    break;
                }
            }

            return neighborNotation;
        }

        std::string buildActionString(const std::string &tileNotation, const std::string &neighborNotation, const Position &newPosition) const
        {
            std::stringstream ss;
            ss << tileNotation;

            Position direction = getDirection(newPosition);

            if (direction == E)
                ss << " " << neighborNotation << "-";
            else if (direction == W)
                ss << " -" << neighborNotation;
            else if (direction == NE)
                ss << " " << neighborNotation << "/";
            else if (direction == NW)
                ss << " \\" << neighborNotation;
            else if (direction == SE)
                ss << " " << neighborNotation << "\\";
            else if (direction == SW)
                ss << " /" << neighborNotation;

            return ss.str();
        }

        std::string getTileNotation(char tileType)
        {
            std::string color = (currentTurn == 'W') ? "W" : "B";
            if (tileType == 'Q')
            {
                return color + tileType;
            }

            std::ostringstream os;
            os << currentTurn << tileType;
            std::string tileNotation = color + tileType + std::to_string(tileNumber[os.str()]);
            return tileNotation;
        }

        Action createAction(const std::string &tileNotation, const std::string &destinationNotation,
                            const std::string &direction, const std::string &side) const
        {
            Tile tile = board.getTileByNotation(tileNotation);
            Tile destinationTile = board.getTileByNotation(destinationNotation);
            Position position = calculatePosition(destinationTile.position, direction, side);
            return Action(tile, position);
        }

        Position calculatePosition(const Position &neighborPosition, const std::string &direction, const std::string &side) const
        {
            Position newPosition = neighborPosition;

            switch (direction[0])
            {
            case '-':
                if (side == "left")
                {
                    newPosition += W;
                }
                else
                {
                    newPosition += E;
                }
                break;

            case '\\':
                if (side == "left")
                {
                    newPosition += NW;
                }
                else
                {
                    newPosition += SE;
                }
                break;

            case '/':
                if (side == "left")
                {
                    newPosition += SW;
                }
                else
                {
                    newPosition += NE;
                }
                break;

            default:

                break;
            }

            return newPosition;
        }

        Position getDirection(const Position &newPosition) const
        {
            Position direction = invalidPosition;
            std::set<Position> directions = {NE, NW, E, W, SE, SW};

            for (const auto &neighbor : board.getNeighbours(newPosition))
            {
                if (!board.isEmpty(neighbor))
                {
                    direction = newPosition - neighbor;
                    break;
                }
            }

            return direction;
        }
    };
}