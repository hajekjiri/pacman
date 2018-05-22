/// Implementation of Portal
/**
 * @file Portal.cpp
 */

 #include <utility>
#include "Portal.h"
#include "Map.h"
#include "MovingGameObject.h"
#include "Pacman.h"
#include "Ghost.h"

Portal::Portal( const int & id,
                const std::pair<int, int> & coords )
              : SolidGameObject( '@', coords ),
                m_Id( id ) {
  /*
   * TODO:
   * - pair portal
   */
}

Portal::~Portal() {
  // TODO
}

void Portal::InteractDefault( MovingGameObject & o, Map & map ) {
  // TODO
}

void Portal::Interact( Ghost & o, Map & map ) {
  // TODO
}

void Portal::Interact( Pacman & o, Map & map ) {
  // TODO
}

void Portal::SetPair( Portal * p ) {
  m_Pair = p;
}

int Portal::GetId() const {
  return m_Id;
}
