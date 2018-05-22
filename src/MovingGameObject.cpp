/// Implementation of MovingGameObject
/**
 * @file MovingGameObject.cpp
 */

#include "MovingGameObject.h"

const int MovingGameObject::DIRECTION_UP = 'w';
const int MovingGameObject::DIRECTION_LEFT = 'a';
const int MovingGameObject::DIRECTION_DOWN = 's';
const int MovingGameObject::DIRECTION_RIGHT = 'd';

MovingGameObject::MovingGameObject( const char & c,
                                    const std::pair<int, int> & coords,
                                    const int & speed,
                                    const bool & lethal )
                                  : GameObject( c, coords ) {
  // TODO
}

MovingGameObject::~MovingGameObject() {
  // TODO
}

const bool MovingGameObject::IsLethal() const {
  return m_Lethal;
}
