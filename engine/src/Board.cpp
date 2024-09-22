#include "Board.hpp"

namespace hive
{

    Board::Board()
    {
        resetBoard();
    }

    Tile Board::getTile(std::pair<int, int> position)
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

    void Board::removeTile(std::pair<int, int> position)
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

    void Board::addTile(std::pair<int, int> position, Tile tile)
    {
        tile.setPosition(position);

        if (isEmpty(position))
        {
            boardTiles[position] = std::list<Tile>();
        }

        boardTiles[position].push_back(tile);
        addEmptyTilesAroundBoard();
    }

    std::set<std::pair<int, int>> Board::getNeighbours(std::pair<int, int> position)
    {
        std::set<std::pair<int, int>> directions = {N, S, NE, SW, NW, SE};
        std::set<std::pair<int, int>> neighbours;

        for (auto &dir : directions)
        {
            neighbours.insert(position + dir);
        }

        return neighbours;
    }


    /*
    TA FUNKCJA KŁAMIE !!!!!!
    TRZEBA NAPRAWIĆ
    */
    std::set<std::pair<int, int>> Board::getAvailableMoves(Tile tile)
    {
        static std::map<TileType, MoveFunc> functions = {
            {TileType::QUEEN, getQueenBeeMoves},
            {TileType::BEETLE, getBeetleMoves},
            {TileType::SPIDER, getSpiderMoves},
            {TileType::GRASSHOPPER, getGrasshopperMoves},
            {TileType::ANT, getAntMoves}};
        return functions[tile.type](*this, tile.position);
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

    bool Board::isEmpty(std::pair<int, int> neighbourPosition)
    {
        return boardTiles.find(neighbourPosition) == boardTiles.end();
    }

    bool Board::isOccupiedByOpponent(std::pair<int, int> pos, Color color)
    {
        Color opponent = color == Color::WHITE ? Color::BLACK : Color::WHITE;
        return calculateNeighbours(pos, opponent) > 0;
    }

    int Board::calculateNeighbours(std::pair<int, int> position, Color color)
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
}