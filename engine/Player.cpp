#include "Player.h"
#include "Pieces/Ant.h"
#include "Pieces/Beetle.h"
#include "Pieces/GrassHopper.h"
#include "Pieces/Spider.h"
#include "Pieces/QueenBee.h"

hge::Player::Player(){
    makePieces();
}

void hge::Player::makePieces(){
    pieces = {hge::QueenBee(),
              hge::Spider(),
              hge::Spider(),
              hge::Beetle(),
              hge::Beetle(),
              hge::Beetle(),
              hge::GrassHopper(),
              hge::GrassHopper(),
              hge::GrassHopper(),
              hge::Ant(),
              hge::Ant()};
}