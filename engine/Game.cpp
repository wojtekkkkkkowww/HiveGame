#include "Game.h"

namespace hge {

    Game::Game() : currentTurn(Turn::PLAYER1) {
        // Constructor implementation
        start();
    }

    Game::~Game() {
        // Destructor implementation
    }

    void Game::start() {
        currentTurn = Turn::PLAYER1;
        
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

} // namespace hge