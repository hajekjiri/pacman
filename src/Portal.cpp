/// Implementation of Protal
/**
 * @file Portal.cpp
 */

#include "Portal.h"

Portal::Portal( const char & id, const std::pair<int, int> & coords )
              : m_Id( id ),
                m_Coords( coords ) {
  // do nothing
}

Portal::~Portal() {
  // do nothing
}

const char & Portal::GetIdConst() const {
  return m_Id;
}

const std::pair<int, int> & Portal::GetCoordsConst() const {
  return m_Coords;
}

std::pair<int, int> & Portal::GetPairCoords() {
  return m_PairCoords;
}
