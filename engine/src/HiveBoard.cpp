#include "HiveBoard.hpp"

hge::HiveBoard::HiveBoard()
{
    resetBoard();
}

void hge::HiveBoard::addEmptyTilesAroundBoard()
{
    emptyTiles.clear();
    for (auto [key, pieceList] : pieces)
    {
        auto piece = pieceList.front();
        for (auto neighbourPosition : piece->getNeighbours())
        {
            if (isEmpty(neighbourPosition))
            {
                addEmptyTile(neighbourPosition);
            }
        }
    }
}

bool hge::HiveBoard::isEmpty(std::pair<int, int> neighbourPosition)
{
    return pieces.find(neighbourPosition) == pieces.end();
}

void hge::HiveBoard::resetBoard()
{
    pieces.clear();
    emptyTiles.clear();
    addEmptyTile(std::make_pair(0, 0));
}

void hge::HiveBoard::addEmptyTile(std::pair<int, int> position)
{
    EmptyTile tile;
    tile.position = position;
    emptyTiles[position] = tile;
}

void hge::HiveBoard::addPiece(std::pair<int, int> position, std::shared_ptr<Piece> piece)
{
    piece->position = position;

    if (isEmpty(position))
    {
        pieces[position] = std::list<std::shared_ptr<Piece>>();
    }

    pieces[position].push_back(piece);
}
