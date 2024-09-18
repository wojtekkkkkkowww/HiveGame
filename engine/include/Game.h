#ifndef GAME_H
#define GAME_H

#include <memory>
#include "Player.h"
#include "HiveBoard.h"

namespace hge {

    
    class Game {
    public:
        Game();
        ~Game();

        void reset();
        PlayerColor getTurn() const;

        PlayerColor currentTurn;
        Player player1;
        Player player2;
        std::shared_ptr<HiveBoard> board;

    };

} 

#endif