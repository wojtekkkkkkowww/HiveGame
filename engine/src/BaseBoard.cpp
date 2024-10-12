#include "BaseBoard.hpp"

namespace hive
{
    void BaseBoard::resetBoard()
    {
        boardTiles.clear();
        emptyTiles.clear();
        emptyTiles.insert({0, 0});
    }

    Tile BaseBoard::getTile(Position position) const
    {
        if (isEmpty(position))
        {
            throw std::invalid_argument("No tile at position");
        }

        const auto &tiles = boardTiles.at(position);
        return tiles.back();
    }

    void BaseBoard::removeTile(Position position)
    {
        if (!isEmpty(position))
        {
            boardTiles[position].pop_back();
            if (boardTiles[position].empty())
            {
                boardTiles.erase(position);
            }
        }
    }

    void BaseBoard::addTile(Position position, Tile &tile)
    {
        tile.setPosition(position);

        if (isEmpty(position))
        {
            boardTiles[position] = std::deque<Tile>();
        }

        boardTiles[position].push_back(tile);
        addEmptyTilesAroundBoard();
    }

    bool BaseBoard::isEmpty(Position position) const
    {
        return boardTiles.find(position) == boardTiles.end();
    }

    int BaseBoard::getLevel(Position position) const
    {
        if (isEmpty(position))
        {
            return 0;
        }

        return boardTiles.at(position).size();
    }

    std::set<Position> BaseBoard::getPlayerTiles(std::string color) const
    {
        std::set<Position> playerTiles;
        for (const auto &[key, tiles] : boardTiles)
        {
            const auto &tile = tiles.back();
            if (tile.color == color)
            {
                playerTiles.insert(tile.position);
            }
        }
        return playerTiles;
    }

    void BaseBoard::addEmptyTilesAroundBoard()
    {
        emptyTiles.clear();
        for (const auto &[key, tiles] : boardTiles)
        {
            const auto &tile = tiles.back();
            for (const auto &neighbourPosition : getNeighbours(tile.position))
            {
                if (isEmpty(neighbourPosition))
                {
                    emptyTiles.insert(neighbourPosition);
                }
            }
        }
    }

    int BaseBoard::calculateNeighbours(Position position, std::string color) const
    {
        int count = 0;
        auto neighbours = getNeighbours(position);
        for (const auto &neighbour : neighbours)
        {
            if (!isEmpty(neighbour))
            {
                const auto &tile = getTile(neighbour);
                if (tile.color == color)
                {
                    count++;
                }
            }
        }
        return count;
    }

    bool BaseBoard::isDirectionBlocked(Position position, Position direction) const
    {
        std::map<Position, std::vector<Position>> neighboringDirections = {
            {N, {N, NE}},
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

        if (getLevel(position) > 1)
        {
            return !getLevel(neighborPosition1) == getLevel(position) && !getLevel(neighborPosition2) == getLevel(position);
        }

        return !isEmpty(neighborPosition1) && !isEmpty(neighborPosition2); // nawet pokusiłbym się o usunięcie tego
    }

    std::set<Position> BaseBoard::getNeighbours(Position position)
    {
        std::set<Position> directions = {N, S, NE, SW, NW, SE};
        std::set<Position> neighbours;

        for (auto &dir : directions)
        {
            neighbours.insert(position + dir);
        }

        return neighbours;
    }

}