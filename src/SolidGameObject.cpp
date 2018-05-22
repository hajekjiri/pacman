/// Implementation of SolidGameObject
/**
 * @file SolidGameObject.cpp
 */

#include "SolidGameObject.h"
#include "Ghost.h"
#include "Pacman.h"

SolidGameObject::SolidGameObject( const char & c,
                                  const std::pair<int, int> & coords )
                                : GameObject( c, coords ) {
  // TODO
}

SolidGameObject::~SolidGameObject() {
  // TODO
}
