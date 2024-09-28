#include "Board.hpp"
#include "dfs.hpp"

namespace hive
{

    Board::Board()
    {
        resetBoard();
    }

    Tile Board::getTile(Position position)
    {
        if (isEmpty(position))
        {
            throw std::invalid_argument("No tile at position");
        }
        auto tileList = boardTiles[position];
        auto tile = tileList.front();
        return tile;
    }

    void Board::resetBoard()
    {
        boardTiles.clear();
        emptyTiles.clear();
        emptyTiles.insert({0, 0});
    }

    void Board::removeTile(Position position)
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

    void Board::addTile(Position position, Tile &tile)
    {
        tile.setPosition(position);

        if (isEmpty(position))
        {
            boardTiles[position] = std::list<Tile>();
        }

        boardTiles[position].push_back(tile);
        addEmptyTilesAroundBoard();
    }

    std::set<Position> Board::getNeighbours(Position position)
    {
        std::set<Position> directions = {N, S, NE, SW, NW, SE};
        std::set<Position> neighbours;

        for (auto &dir : directions)
        {
            neighbours.insert(position + dir);
        }

        return neighbours;
    }

    std::set<Position> Board::getAvailableMoves(Tile tile)
    {
        static std::map<std::string, MoveFunc> functions = {
            {"QUEEN", getQueenBeeMoves},
            {"BEETLE", getBeetleMoves},
            {"SPIDER", getSpiderMoves},
            {"GRASSHOPPER", getGrasshopperMoves},
            {"ANT", getAntMoves}};
        return functions[tile.type](*this, tile.position);
    }

    std::set<Position> Board::getPlayerTiles(std::string color)
    {
        std::set<Position> playerTiles;
        for (auto [key, tileList] : boardTiles)
        {
            auto tile = tileList.front();
            if (tile.color == color)
            {
                playerTiles.insert(tile.position);
            }
        }
        return playerTiles;
    }

    void Board::addEmptyTilesAroundBoard()
    {
        emptyTiles.clear();
        for (auto [key, tileList] : boardTiles)
        {
            auto tile = tileList.front();
            for (auto neighbourPosition : getNeighbours(tile.position))
            {
                if (isEmpty(neighbourPosition))
                {
                    emptyTiles.insert(neighbourPosition);
                }
            }
        }
    }

    /*
    TO jest okej bo zabranie figury moze rozerwać rój
    jednagrze sam ruch też może to zrobić
    */

    bool Board::isHiveConnectedAfterRemove(Position position)
    {
        Tile tile = getTile(position);
        removeTile(position);

        DFS dfs(*this);
        std::set<Position> visited = dfs.performDFS(boardTiles.begin()->first);

        std::set<Position> tilePositions;
        for (const auto &[pos, _] : boardTiles)
        {
            tilePositions.insert(pos);
        }

        addTile(position, tile);

        return visited == tilePositions;
    }

    bool Board::isDirectionBlocked(Position position, Position direction)
    {
        std::map<Position, std::vector<Position>> neighboringDirections = {
            {{0, 1}, {{1, 1}, {-1, 1}}},    // N -> NE, NW
            {{1, 1}, {{0, 1}, {1, -1}}},    // NE -> N, SE
            {{1, -1}, {{1, 1}, {0, -1}}},   // SE -> NE, S
            {{0, -1}, {{1, -1}, {-1, -1}}}, // S -> SE, SW
            {{-1, -1}, {{0, -1}, {-1, 1}}}, // SW -> S, NW
            {{-1, 1}, {{0, 1}, {-1, -1}}}   // NW -> N, SW
        };

        if (neighboringDirections.find(direction) == neighboringDirections.end())
        {
            std::cerr << "Error: Invalid direction provided." << std::endl;
            return true;
        }
        auto neighbors = neighboringDirections[direction];

        Position neighborPosition1 = {position.x + neighbors[0].x, position.y + neighbors[0].y};
        Position neighborPosition2 = {position.x + neighbors[1].x, position.y + neighbors[1].y};

        return !isEmpty(neighborPosition1) && !isEmpty(neighborPosition2);
    }

    bool Board::isTouchingHiveAfterMove(Position position, Position newPosition)
    {
        for (auto neighbour : getNeighbours(newPosition))
        {
            if (!isEmpty(neighbour) && neighbour != position)
            {
                return true;
            }
        }
        return false;
    }

    bool Board::isMoveBlocked(Position position, Position newPosition)
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

        if (tile.type == "ANT")
        {
            return calculateNeighbours(newPosition, tile.color) >= 3;
        }
        else
        {
            return isDirectionBlocked(position, direction);
        }
    }

    bool Board::isEmpty(Position neighbourPosition)
    {
        return boardTiles.find(neighbourPosition) == boardTiles.end();
    }

    bool Board::isOccupiedByOpponent(Position pos, std::string color)
    {
        std::string opponent = color == "WHITE" ? "BLACK" : "WHITE";
        return calculateNeighbours(pos, opponent) > 0;
    }

    int Board::calculateNeighbours(Position position, std::string color)
    {
        int count = 0;
        auto neighbours = getNeighbours(position);
        for (auto neighbour : neighbours)
        {
            if (!isEmpty(neighbour))
            {
                auto tile = getTile(neighbour);
                if (tile.color == color)
                {
                    count++;
                }
            }
        }
        return count;
    }

    int Board::getLevel(Position position)
    {
        return boardTiles[position].size();
    }

    bool Board::isQueenSurrounded(std::string color)
    {
        std::string opponent = color == "WHITE" ? "BLACK" : "WHITE";
        for (auto [key, tileList] : boardTiles)
        {
            auto tile = tileList.front();
            if (tile.color == color && tile.type == "QUEEN")
            {
                return calculateNeighbours(tile.position, color) + calculateNeighbours(tile.position, opponent) == 6;
            }
        }
        return false;
    }
}