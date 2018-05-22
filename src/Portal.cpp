/// Implementation of Portal
/**
 * @file Portal.cpp
 */

#include "Portal.h"
#include <utility>

Portal::Portal( const int & id,
                std::pair<int, int> coords )
              : SolidGameObject( '@' ),
                m_Id( id ),
                m_Coords( coords ) {
  /*
   * TODO:
   * - pair portal
   */
}

Portal::~Portal() {
  // TODO
}

void Portal::Interact( MovingGameObject & o ) {
  // TODO
}

void Portal::Interact( Ghost & o ) {
  // TODO
}

void Portal::Interact( Pacman & o ) {
  // TODO
}

void Portal::SetPair( Portal * p ) {
  m_Pair = p;
}

int Portal::GetId() const {
  return m_Id;
}
