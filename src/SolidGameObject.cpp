/// Implementation of SolidGameObject
/**
 * @file SolidGameObject.cpp
 */

#include "SolidGameObject.h"

SolidGameObject::SolidGameObject( const char & c,
                                  const std::pair<int, int> & coords )
                                : GameObject( c, coords ) {
  // TODO
}

SolidGameObject::~SolidGameObject() {
  // TODO
}

void SolidGameObject::Interact( MovingGameObject & o ) {
  return;
}
