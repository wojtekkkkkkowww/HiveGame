#pragma once

#include <memory>
#include "Player.hpp"
#include "HiveBoard.hpp"

namespace hge
{

    class Game
    {
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

