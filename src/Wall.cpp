/// Implementation of Wall
/**
 * @file Wall.cpp
 */

#include "Wall.h"
#include "Map.h"

Wall::Wall( const std::pair<int, int> & coords )
          : SolidGameObject( '#', coords ) {
  // TODO
}

Wall::~Wall() {
  // TODO
}

void Wall::Interact( Ghost & o, Map & map ) {
  // TODO
}

void Wall::Interact( Pacman & o, Map & map ) {
  // TODO
}
