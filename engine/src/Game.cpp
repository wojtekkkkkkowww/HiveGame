#include "Game.hpp"

namespace hive
{
    Game::Game() : currentTurn(Color::WHITE)
    {
        players[Color::WHITE] = Player(Color::WHITE);
        players[Color::BLACK] = Player(Color::BLACK);
        startNewGame();
    }

    Game::~Game()
    {
    }

    void Game::moveTile(std::pair<int, int> position, std::pair<int, int> newPosition)
    {
        auto tile = board.getTile(position);

        if (tile.color != currentTurn)
        {
            throw std::invalid_argument("This piece does not belong to the player");
        }

        auto availableMoves = board.getAvailableMoves(tile);

        if (availableMoves.find(newPosition) == availableMoves.end())
        {
            throw std::invalid_argument("This move is not allowed");
        }

        board.removeTile(position);
        board.addTile(newPosition, tile);
        switchTurn();
    }

    void Game::placeTile(std::pair<int, int> position, TileType type)
    {
        Color opponent = (currentTurn == Color::WHITE) ? Color::BLACK : Color::WHITE;

        checkPositionAvailability(players[currentTurn], opponent, position);

        auto tile = createTile(type, currentTurn);

        board.addTile(position, tile);
        if (type == TileType::QUEEN)
        {
            players[currentTurn].queenPlaced = true;
            players[currentTurn].firstMove = false;
        }
        switchTurn();
    }

    void Game::checkPositionAvailability(const Player &player, Color opponent, std::pair<int, int> position)
    {
        if ((!board.isEmpty(position) || board.isOccupiedByOpponent(position, opponent)) && !player.firstMove)
        {
            throw std::invalid_argument("This position is not available");
        }
    }

    void Game::startNewGame()
    {
        currentTurn = Color::WHITE;
        board.resetBoard();
    }

    Tile Game::createTile(TileType type, Color color)
    {
        return players[color].takeTile(type);
    }

    void Game::switchTurn()
    {
        currentTurn = (currentTurn == Color::WHITE) ? Color::BLACK : Color::WHITE;
    }
}