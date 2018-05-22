/// Implementation of Blank
/**
 * @file Blank.cpp
 */

#include "Blank.h"

Blank::Blank( const std::pair<int, int> & coords )
            : SolidGameObject( ' ', coords ) {
  // TODO
}

Blank::~Blank() {
  // TODO
}

void Blank::Interact( Ghost & o ) {
  // TODO
}

void Blank::Interact( Pacman & o ) {
  // TODO
}
