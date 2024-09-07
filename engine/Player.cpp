#include "Player.h"
#include "Tiles/Ant.h"
#include "Tiles/Beetle.h"
#include "Tiles/GrassHopper.h"
#include "Tiles/Spider.h"
#include "Tiles/QueenBee.h"

hge::Player::Player(){
    makeTiles();
}

void hge::Player::makeTiles(){
    tiles = {hge::QueenBee(),
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