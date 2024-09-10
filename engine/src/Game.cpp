#include "Game.h"

namespace hge {

    Game::Game() : currentTurn(Turn::PLAYER1), board(std::make_shared<HiveBoard>()), player1(board), player2(board) {
        start();
    }

    Game::~Game() {
    }

    void Game::start() {
        currentTurn = Turn::PLAYER1;
        board->resetBoard();
        /*
        clear board
        */

        /*
        craete pieces for both players
        */
    }


    Turn Game::getTurn() const {
        return currentTurn;
    }

}