/// Implementation of Ghost
/**
 * @file Ghost.cpp
 */

#include "Ghost.h"

Ghost::Ghost( const char & c,
              const std::pair<int, int> & coords,
              const int & speed,
              const bool & lethal )
            : MovingGameObject( c, coords, speed, lethal ) {
  // TODO
}

Ghost::~Ghost() {
 // TODO
}

void Ghost::Interact( Ghost & o ) {
  // TODO
}

void Ghost::Interact( Pacman & o ) {
  // TODO
}

void Ghost::Move( const int & direction ) {
  // TODO
}
