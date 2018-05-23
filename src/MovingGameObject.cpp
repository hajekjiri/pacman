/// Implementation of MovingGameObject
/**
 * @file MovingGameObject.cpp
 */

#include "MovingGameObject.h"

MovingGameObject::MovingGameObject( const char & c,
                                    const std::pair<int, int> & coords,
                                    const int & speed,
                                    const bool & lethal )
                                  : GameObject( c ),
                                    m_Coords( coords ),
                                    m_Lethal( lethal ),
                                    m_Alive( true ),
                                    m_Speed( speed ) {
  // do nothing
}

MovingGameObject::~MovingGameObject() {
  // do nothing
}

std::pair<int, int> & MovingGameObject::Coords() {
  return m_Coords;
}

bool & MovingGameObject::Lethal() {
  return m_Lethal;
}

bool & MovingGameObject::Alive() {
  return m_Alive;
}

int & MovingGameObject::Speed() {
  return m_Speed;
}
