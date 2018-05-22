/// Implementation of Pacman
/**
 * @file Pacman.cpp
 */

#include "Pacman.h"

Pacman::Pacman( const std::pair<int, int> & coords,
                const int & speed,
                const bool & lethal )
              : MovingGameObject( 'P', coords, speed, lethal ) {
  // TODO
}

Pacman::~Pacman() {
 // TODO
}

void Pacman::Interact( Ghost & o ) {
  // TODO
}

void Pacman::Interact( Pacman & o ) {
  // TODO
}
