#pragma once

#include "Action.hpp"
#include "Board.hpp"
#include "Player.hpp"
#include "Position.hpp"
#include "Tile.hpp"

#include <optional>
#include <regex>
#include <sstream>

namespace hive
{

/**
 * @brief The ActionParser class is responsible for converting between string notations and Action
 * objects.
 */
class ActionParser
{
  public:
    ActionParser(const Board &board, const PlayerColor &currentTurn, const Players &players)
        : board(board), currentTurn(currentTurn), players(players)
    {
    }

    /**
     * @brief Converts a notation string to an Action object.
     * @param notation The string notation, e.g., "WG2-bQ".
     * @return The corresponding Action object.
     */
    std::optional<Action> stringToAction(const std::string &notation) const
    {
        static const std::regex rightDirectionRegex(R"(([WB][A-Z]\d?) ([WB][A-Z]\d?)([-\\/]))");
        static const std::regex beetleMoveRegex(R"(([WB][A-Z]\d?) ([WB][A-Z]\d?)\.)");
        static const std::regex leftDirectionRegex(R"(([WB][A-Z]\d?) ([-\\/])([WB][A-Z]\d?))");
        static const std::regex firstActionRegex(R"([WB][A-Z]\d?)");
        std::smatch matches;

        if (std::regex_match(notation, matches, leftDirectionRegex))
        {
            return parseLeftDirection(matches);
        }
        if (std::regex_match(notation, matches, rightDirectionRegex))
        {
            return parseRightDirection(matches);
        }
        if (std::regex_match(notation, matches, beetleMoveRegex))
        {
            return parseBeetleMove(matches);
        }
        if (std::regex_match(notation, matches, firstActionRegex))
        {
            return parseFirstAction(matches);
        }

        if (notation == "Wait")
        {
            return makeWaitAction();
        }
        return std::nullopt;
    }

    /**
     * @brief Converts an Action object to the corresponding string notation.
     * @param action The Action object to convert.
     * @return The string notation, e.g., "WG2-bQ".
     */
    std::string actionToString(const Action &action)
    {
        if (action.type == ActionType::WAIT)
        {
            return "Wait";
        }

        std::string tileNotation;
        if (action.type == ActionType::MOVE)
        {
            tileNotation = getMoveTileNotation(action);
        }
        else if (action.type == ActionType::PLACE)
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

  private:
    const Board &board;
    const PlayerColor &currentTurn;
    const Players &players;

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

        return makePlaceAction({0, 0}, pieceTypeFromChar(tileNotation[1]));
    }

    Action parseBeetleMove(const std::smatch &matches) const
    {
        std::string tileNotation = matches[1].str();
        std::string destinationNotation = matches[2].str();

        Tile tile = board.getTileByNotation(tileNotation);
        Tile destinationTile = board.getTileByNotation(destinationNotation);
        Position position = destinationTile.position;

        return makeAction(tile, position);
    }

    std::string getMoveTileNotation(const Action &action) const
    {
        Tile tile = board.getTile(action.position);
        return tile.notation;
    }

    std::string getPlaceTileNotation(const Action &action)
    {
        const char color = toChar(currentTurn);
        const char pieceType = toChar(action.pieceType);
        if (action.pieceType == PieceType::Queen)
        {
            return std::string{color, pieceType};
        }

        const int count = players[currentTurn].getPlacedPieceCount(action.pieceType) + 1;

        std::stringstream tileNotation;
        tileNotation << color << pieceType << count;

        return tileNotation.str();
    }

    std::string getNeighborNotation(const Position &newPosition) const
    {
        std::string neighborNotation;

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

    std::string buildActionString(const std::string &tileNotation,
                                  const std::string &neighborNotation,
                                  const Position &newPosition) const
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

    Action createAction(const std::string &tileNotation, const std::string &destinationNotation,
                        const std::string &direction, const std::string &side) const
    {
        const Tile tile = board.getTileByNotation(tileNotation);
        const Tile destinationTile = board.getTileByNotation(destinationNotation);
        Position position = calculatePosition(destinationTile.position, direction, side);
        return makeAction(tile, position);
    }

    Position calculatePosition(const Position &neighborPosition, const std::string &direction,
                               const std::string &side) const
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
} // namespace hive
