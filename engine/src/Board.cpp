#include "Board.hpp"
#include <algorithm>
#include <array>
#include <queue>
#include <stack>
#include <stdexcept>
#include <utility>

namespace hive
{

void Board::resetBoard()
{
    pieces = {};
    emptyTiles.clear();
    emptyTiles.insert({0, 0});
    blackQueen = invalidPosition;
    whiteQueen = invalidPosition;
}

void Board::setBoardTiles(const std::map<Position, std::deque<Tile>> &tiles)
{
    resetBoard();
    for (const auto &[position, tile] : tiles)
    {
        for (const auto &t : tile)
        {
            addTile(position, t);
        }
    }

    addEmptyTilesAroundBoard();
}

std::vector<std::list<Tile>> Board::getTiles() const
{
    size_t maxLevel = 0;
    for (const auto &piece : pieces)
    {
        if (piece.active)
        {
            maxLevel = std::max(static_cast<std::size_t>(piece.level) + 1, maxLevel);
        }
    }

    std::vector<std::list<Tile>> tiles(maxLevel);

    for (const auto &piece : pieces)
    {
        if (piece.active)
        {
            tiles[piece.level].push_back(piece.tile);
        }
    }

    return tiles;
}

std::set<Position> Board::getPlayerTiles(char color) const
{
    std::set<Position> playerTiles;
    for (const auto &position : getPositions())
    {
        const auto tile = getTile(position);
        if (tile.color == color)
        {
            playerTiles.insert(tile.position);
        }
    }
    return playerTiles;
}

std::vector<Position> Board::getPositions() const
{
    std::vector<Position> positions;
    positions.reserve(getPieceCount());
    for (const auto &piece : pieces)
    {
        if (piece.active &&
            std::find(positions.begin(), positions.end(), piece.tile.position) == positions.end())
        {
            positions.push_back(piece.tile.position);
        }
    }
    return positions;
}

void Board::addEmptyTilesAroundBoard()
{
    emptyTiles.clear();
    for (const auto &position : getPositions())
    {
        for (const auto &dir : directions)
        {
            Position neighbour = position + dir;
            if (isEmpty(neighbour))
            {
                emptyTiles.insert(neighbour);
            }
        }
    }
}

std::set<Position> Board::getNeighbours(Position position)
{
    std::set<Position> neighbours;

    for (const auto &dir : directions)
    {
        neighbours.insert(position + dir);
    }

    return neighbours;
}

Tile Board::getTileByNotation(const std::string &notation) const
{
    const auto *piece = findPieceByNotation(notation);
    if (piece == nullptr)
    {
        return {notation};
    }
    return piece->tile;
}

int Board::getLevel(Position position) const
{
    int level = 0;
    for (const auto &piece : pieces)
    {
        if (piece.active && piece.tile.position == position)
        {
            ++level;
        }
    }
    return level;
}

void Board::moveTile(Position position, Position newPosition)
{
    auto *piece = findTopPiece(position);
    if (piece == nullptr)
    {
        throw std::out_of_range("No tile at position");
    }

    const auto destinationLevel = getLevel(newPosition);
    piece->tile.setPosition(newPosition);
    piece->level = static_cast<std::uint8_t>(destinationLevel);
}

int Board::calculateColoredNeighbours(Position position, char color) const
{
    int count = 0;
    for (const auto &dir : directions)
    {
        if (!isEmpty(position + dir))
        {
            const auto &tile = getTile(position + dir);
            if (tile.color == color)
            {
                count++;
            }
        }
    }
    return count;
}

int Board::calculateNeighbours(const Position &position) const
{
    int count = 0;
    for (const auto &dir : directions)
    {
        if (!isEmpty(position + dir))
        {
            count++;
        }
    }
    return count;
}

Tile Board::getTile(Position position) const
{
    const auto *piece = findTopPiece(position);
    if (piece == nullptr)
    {
        throw std::out_of_range("No tile at position");
    }
    return piece->tile;
}

void Board::removeTile(Position position)
{
    if (auto *piece = findTopPiece(position))
    {
        *piece = {};
    }
}

void Board::addTile(Position position, Tile tile)
{
    tile.setPosition(position);

    if (tile.type == 'Q' && tile.color == 'W')
    {
        whiteQueen = position;
    }
    if (tile.type == 'Q' && tile.color == 'B')
    {
        blackQueen = position;
    }

    auto emptyPiece = std::find_if(pieces.begin(), pieces.end(),
                                   [](const PieceState &piece) { return !piece.active; });
    if (emptyPiece == pieces.end())
    {
        throw std::length_error("Board cannot contain more than 22 pieces");
    }

    *emptyPiece = PieceState{.tile = std::move(tile),
                             .level = static_cast<std::uint8_t>(getLevel(position)),
                             .active = true};
}

int Board::getTileCount() const
{
    int occupiedPositions = 0;
    for (std::size_t index = 0; index < pieces.size(); ++index)
    {
        if (!pieces[index].active)
        {
            continue;
        }

        bool seenEarlier = false;
        for (std::size_t previous = 0; previous < index; ++previous)
        {
            if (pieces[previous].active &&
                pieces[previous].tile.position == pieces[index].tile.position)
            {
                seenEarlier = true;
                break;
            }
        }
        if (!seenEarlier)
        {
            ++occupiedPositions;
        }
    }
    return occupiedPositions;
}

std::size_t Board::getPieceCount() const
{
    return static_cast<std::size_t>(
        std::count_if(pieces.begin(), pieces.end(), [](const PieceState &piece) {
            return piece.active;
        }));
}

Board::PieceState *Board::findTopPiece(Position position)
{
    return const_cast<PieceState *>(std::as_const(*this).findTopPiece(position));
}

const Board::PieceState *Board::findTopPiece(Position position) const
{
    const PieceState *top = nullptr;
    for (const auto &piece : pieces)
    {
        if (piece.active && piece.tile.position == position &&
            (top == nullptr || piece.level > top->level))
        {
            top = &piece;
        }
    }
    return top;
}

Board::PieceState *Board::findPieceByNotation(const std::string &notation)
{
    return const_cast<PieceState *>(std::as_const(*this).findPieceByNotation(notation));
}

const Board::PieceState *Board::findPieceByNotation(const std::string &notation) const
{
    const auto piece = std::find_if(pieces.begin(), pieces.end(), [&](const PieceState &state) {
        return state.active && state.tile.notation == notation;
    });
    return piece == pieces.end() ? nullptr : &*piece;
}

// movement manager
void Board::getAvailableMoves(char type, const Position &position, std::set<Position> &outMoves)
{
    outMoves.clear();
    switch (type)
    {
    case 'G':
        getGrasshopperMoves(position, outMoves);
        break;
    case 'Q':
        getQueenMoves(position, outMoves);
        break;
    case 'B':
        getBeetleMoves(position, outMoves);
        break;
    case 'S':
        getSpiderMoves(position, outMoves);
        break;
    case 'A':
        getAntMoves(position, outMoves);
        break;
    }
}

/*
Ant:
Can move to any tile it can physically reach.
DFS on tiles that are in the emptyTiles set.
The list of visited tiles is the possible moves for the ant.
*/
void Board::getAntMoves(const Position &position, std::set<Position> &outMoves)
{
    outMoves.clear();

    std::stack<Position> stack;

    for (const auto &direction : directions)
    {
        Position neighbour = position + direction;
        if (emptyTiles.find(neighbour) != emptyTiles.end() &&
            !isDirectionBlocked(position, direction, 1))
        {
            stack.push(neighbour);
        }
    }

    while (!stack.empty())
    {
        Position current = stack.top();
        stack.pop();

        if (outMoves.find(current) != outMoves.end())
        {
            continue;
        }
        outMoves.insert(current);

        for (const auto &direction : directions)
        {
            if (emptyTiles.find(current + direction) != emptyTiles.end() &&
                outMoves.find(current + direction) == outMoves.end() &&
                !isDirectionBlocked(current, direction, 1))
            {
                stack.push(current + direction);
            }
        }
    }
}

/*
BEETLE:
Can move to adjacent tiles on the same level or on diferent level.
*/
void Board::getBeetleMoves(const Position &position, std::set<Position> &outMoves) const
{
    outMoves.clear();

    auto beetle_level = getLevel(position);

    for (const auto &direction : directions)
    {
        if (getLevel(position + direction) >= beetle_level)
        {
            int newBeetleLevel = getLevel(position + direction) + 1;
            if (!isDirectionBlocked(position, direction, newBeetleLevel))
            {
                outMoves.insert(position + direction);
            }
            continue;
        }

        if (!isDirectionBlocked(position, direction, beetle_level))
        {
            outMoves.insert(position + direction);
        }
    }
}

/*
SPIDER
Moves exactly 3 spaces without backtracking and must touch another piece at each intermediate space.
Implementation requires BFS which expands breadth-first and then depth-first to find all paths of
length 3.
*/
void Board::getSpiderMoves(const Position &position, std::set<Position> &outMoves)
{
    outMoves.clear();

    std::set<Position> visited;
    std::queue<std::pair<Position, int>> q;
    std::set<Position> spiderNeighbours = getNeighbours(position);

    q.push({position, 0});
    visited.insert(position);

    while (!q.empty())
    {
        const auto &[currentPos, depth] = q.front();
        q.pop();

        if (depth == 3)
        {
            outMoves.insert(currentPos);
            continue;
        }

        for (const auto &direction : directions)
        {
            Position neighbor = currentPos + direction;
            if (visited.find(neighbor) == visited.end() &&
                emptyTiles.find(neighbor) != emptyTiles.end())
            {
                int isSpiderNeighbour =
                    (spiderNeighbours.find(neighbor) != spiderNeighbours.end()) ? 1 : 0;
                if (calculateNeighbours(neighbor) - isSpiderNeighbour > 0)
                {
                    if (isDirectionBlocked(currentPos, neighbor - currentPos, 1))
                    {
                        continue;
                    }
                    visited.insert(neighbor);
                    q.push({neighbor, depth + 1});
                }
            }
        }
    }
}

/*
GRASSHOPPER:
Jumps over pieces to an empty tile behind them. In any direction.
Must jump over at least one piece.
*/
void Board::getGrasshopperMoves(const Position &position, std::set<Position> &outMoves) const
{
    outMoves.clear();

    for (const auto &direction : directions)
    {
        Position current = position;

        if (isEmpty(current + direction))
        {
            continue;
        }

        while (!isEmpty(current))
        {
            current = current + direction;
        }

        outMoves.insert(current);
    }
}

/*
Queen:
Can move to adjacent tiles
*/
void Board::getQueenMoves(const Position &position, std::set<Position> &outMoves) const
{
    outMoves.clear();

    for (const auto &direction : directions)
    {
        if (isEmpty(position + direction) && !isDirectionBlocked(position, direction, 1))
        {
            outMoves.insert(position + direction);
        }
    }
}

bool Board::isMoveBlocked(Position position, Position newPosition) const
{
    return !isTouchingHiveAfterMove(position, newPosition);
}

bool Board::isOccupiedByOpponent(Position pos, char color) const
{
    char opponent = color == 'W' ? 'B' : 'W';
    return calculateColoredNeighbours(pos, opponent) > 0;
}

bool Board::isQueenSurrounded(char color) const
{
    Position position = (color == 'W') ? whiteQueen : blackQueen;
    if (position == invalidPosition)
    {
        return false;
    }
    return calculateNeighbours(position) == 6;
}

bool Board::isDirectionBlocked(Position position, Position direction, int level) const
{
    const auto neighbors = neighboringDirections(direction);

    if (neighbors[0] == Position{} && neighbors[1] == Position{})
    { // invalid direction
        return true;
    }

    if (fredomToMove(position + neighbors[0], level, position + neighbors[1]))
    {
        return true;
    }

    if (getTileCount() == 1 || level != 1)
    {
        return false;
    }

    if (constantContact(position + neighbors[0], position + neighbors[1], position + direction))
    {
        return true;
    }

    return false;
}

bool Board::constantContact(hive::Position neighborPosition1, hive::Position neighborPosition2,
                            hive::Position newPosition) const
{
    return isEmpty(neighborPosition1) && isEmpty(neighborPosition2) && getLevel(newPosition) == 0;
}

bool Board::fredomToMove(hive::Position neighborPosition1, int level,
                         hive::Position neighborPosition2) const
{
    return getLevel(neighborPosition1) >= level && getLevel(neighborPosition2) >= level;
}

bool Board::isTouchingHiveAfterMove(Position position, Position newPosition) const
{
    for (const auto &neighbour : getNeighbours(newPosition))
    {
        if (!isEmpty(neighbour) && (neighbour != position || getLevel(position) > 1))
        {
            return true;
        }
    }
    return false;
}

// move validator

} // namespace hive
