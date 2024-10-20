#include "MoveValidator.hpp"
#include "dfs.hpp"

namespace hive
{
    bool MoveValidator::isMoveBlocked(Position position, Position newPosition) const
    {

        if (!isHiveConnectedAfterRemove(position))
        {
            return true;
        }

        if (!isTouchingHiveAfterMove(position, newPosition))
        {
            return true;
        }

        Tile tile = getTile(position);
        Position direction = {newPosition.x - position.x, newPosition.y - position.y};

        /*
        // te dwa siupy mogły by iść do klasy MovementManager razem z isDirectionBlocked
        if (tile.type == 'Q')
        {
            return isDirectionBlocked(position, direction);
        }

        if (tile.type == 'B')
        {

            if (getLevel(position) == getLevel(newPosition) + 1)
            {
                return isDirectionBlocked(position, direction);
            }
        }
        */

        return false;
    }

    bool MoveValidator::isOccupiedByOpponent(Position pos, std::string color) const
    {
        std::string opponent = color == "WHITE" ? "BLACK" : "WHITE";
        return calculateNeighbours(pos, opponent) > 0;
    }

    bool MoveValidator::isQueenSurrounded(std::string color) const
    {
        std::string opponent = color == "WHITE" ? "BLACK" : "WHITE";
        Position position = (color == "WHITE") ? whiteQueen : blackQueen;
        if (position == invalidPosition)
        {
            return false;
        }
        return calculateNeighbours(position, color) + calculateNeighbours(position, opponent) == 6;
    }

    bool MoveValidator::isDirectionBlocked(Position position, Position direction, int level) const
    { // powinna wiedziec czy te ziomki co sprawdza sa na tym samym poziomie
        // trrzeba dodac zakas fruwania

        std::map<Position, std::vector<Position>> neighboringDirections = {
            {N, {NW, NE}},
            {NE, {N, SE}},
            {SE, {NE, S}},
            {S, {SE, SW}},
            {SW, {S, NW}},
            {NW, {N, SW}}};

        if (neighboringDirections.find(direction) == neighboringDirections.end())
        {
            std::cerr << "Error: Invalid direction provided. " << direction.x << " " << direction.y << std::endl;
            return true;
        }
        auto neighbors = neighboringDirections[direction];

        Position neighborPosition1 = {position.x + neighbors[0].x, position.y + neighbors[0].y};
        Position neighborPosition2 = {position.x + neighbors[1].x, position.y + neighbors[1].y};

        // FREDOM TO MOVE, nie widze tego
        if (getLevel(neighborPosition1) >= level && getLevel(neighborPosition2) >= level)
        {
            std::cerr<<"Direction "<< direction.x << " " <<direction.y <<" is blocked because:\n";
            std::cerr<<neighborPosition1.x <<" " <<neighborPosition1.y << ">=" << position.x << " " << position.y <<"\n";
            std::cerr<<neighborPosition2.x <<" " <<neighborPosition2.y << ">=" << position.x << " " << position.y <<"\n";
             
            std::cerr << "\033[1;31mITS MY FAULT\033[0m \n";
            return true;
        }

        if(boardTiles.size() == 1)
        {   
            return false;
        }

        // CONSTANT CONTACT
        Position newPosition = position + direction;

        if (isEmpty(neighborPosition1) && isEmpty(neighborPosition2) && level == 1 && getLevel(newPosition) == 0)
        {
            std::cerr << "\033[1;31mITS MY FAULT\033[0m " << newPosition.x << " " << newPosition.y << std::endl;
            return true;
        }
        return false;
    }

    /*
    sprawdzenie wymaga przejścia po wszystkich polach urzywając dfs
    corner case: Koło
    */
    bool MoveValidator::isHiveConnectedAfterRemove(Position position) const
    {
        if (getLevel(position) > 1)
        {
            return true;
        }

        std::set<Position> tilesPositions;
        for (auto [pos, _] : boardTiles)
        {
            if (pos != position)
                tilesPositions.insert(pos);
        }

        DFS dfs(tilesPositions);
        std::set<Position> visited = dfs.performDFS();

        return visited == tilesPositions;
    }

    bool MoveValidator::isTouchingHiveAfterMove(Position position, Position newPosition) const
    {
        for (auto neighbour : getNeighbours(newPosition))
        {
            if (!isEmpty(neighbour) && (neighbour != position || getLevel(position) > 1))
            {
                return true;
            }
        }
        return false;
    }
}