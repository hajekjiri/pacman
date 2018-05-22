/// Implementation of Blank
/**
 * @file Blank.cpp
 */

#include "Blank.h"
#include "Map.h"

Blank::Blank( const std::pair<int, int> & coords )
            : SolidGameObject( ' ', coords ) {
  // TODO
}

Blank::~Blank() {
  // TODO
}

void Blank::Interact( Ghost & o, Map & map ) {
  // TODO
}

void Blank::Interact( Pacman & o, Map & map ) {
  // TODO
}
