/// Implementation of Coin
/**
 * @file Coin.cpp
 */

#include "Coin.h"
#include "Map.h"

Coin::Coin( const std::pair<int, int> & coords )
          : SolidGameObject( '-', coords ) {
  // TODO
}

Coin::~Coin() {
  // TODO
}

void Coin::Interact( Ghost & o, Map & map ) {
  // TODO
}

void Coin::Interact( Pacman & o, Map & map ) {
  // TODO
}
