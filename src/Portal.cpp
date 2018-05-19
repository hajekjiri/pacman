/// Implementation of Portal
/**
 * @file Portal.cpp
 */

#include "Portal.h"

Portal::Portal( const int & id )
              : SolidGameObject( '@' ),
                m_Id( id ) {
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
