#include "Game.h"

namespace hge {

    Game::Game() : currentTurn(Turn::PLAYER1) {
        // Constructor implementation
    }

    Game::~Game() {
        // Destructor implementation
    }

    void Game::start() {
        // Start the game
        currentTurn = Turn::PLAYER1;
    }

    void Game::stop() {
        // Stop the game
    }

    Turn Game::getTurn() const {
        return currentTurn;
    }

} // namespace hge