#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "HiveBoard.h"

namespace hge {

    enum class Turn {
        PLAYER1,
        PLAYER2
    };

    class Game {
    public:
        Game();
        ~Game();

        void start();
        Turn getTurn() const;

        Turn currentTurn;
        Player player1;
        Player player2;
        HiveBoard board;

    };

} // namespace hge

#endif // GAME_H