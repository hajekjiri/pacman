/// Implementation of MovingGameObject
/**
 * @file MovingGameObject.cpp
 */

#include "MovingGameObject.h"

MovingGameObject::MovingGameObject( const char & c,
                                    const int & speed,
                                    const bool & lethal )
                                  : GameObject( c ) {
  // TODO
}

MovingGameObject::~MovingGameObject() {
  // TODO
}

const bool MovingGameObject::IsLethal() const {
  return m_Lethal;
}

void MovingGameObject::Move( const int & direction ) {
  // TODO
}
