/// Implementation of Wall
/**
 * @file Wall.cpp
 */

#include "Wall.h"

Wall::Wall( const std::pair<int, int> & coords )
          : SolidGameObject( '#', coords ) {
  // TODO
}

Wall::~Wall() {
  // TODO
}

void Wall::Interact( Ghost & o ) {
  // TODO
}

void Wall::Interact( Pacman & o ) {
  // TODO
}
