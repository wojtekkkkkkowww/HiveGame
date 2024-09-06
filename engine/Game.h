#ifndef GAME_H
#define GAME_H

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
        void stop();
        Turn getTurn() const;

    private:
        Turn currentTurn;
    };

} // namespace hge

#endif // GAME_H