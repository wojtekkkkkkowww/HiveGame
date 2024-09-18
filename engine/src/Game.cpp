#include "Game.hpp"

namespace hge {

    Game::Game() : currentTurn(PlayerColor::WHITE_PLAYER), board(std::make_shared<HiveBoard>()), player1(board), player2(board) {
        reset();
    }

    Game::~Game() {
    }

    void Game::reset() {
        currentTurn = PlayerColor::WHITE_PLAYER;
        board->resetBoard();
    }


    PlayerColor Game::getTurn() const {
        return currentTurn;
    }

}