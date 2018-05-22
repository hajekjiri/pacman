/// Implementation of Pacman
/**
 * @file Pacman.cpp
 */

#include "Pacman.h"
#include "Map.h"

Pacman::Pacman( const std::pair<int, int> & coords,
                const int & speed,
                const bool & lethal )
              : MovingGameObject( 'P', coords, speed, lethal ) {
  // TODO
}

Pacman::~Pacman() {
 // TODO
}

void Pacman::Interact( Ghost & o, Map & map ) {
  // TODO
}

void Pacman::Interact( Pacman & o, Map & map ) {
  // TODO
}

void Pacman::Move( const int & direction ) {

}
