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

char & Portal::Id() {
  return m_Id;
}

std::pair<int, int> & Portal::Coords() {
  return m_Coords;
}

std::pair<int, int> & Portal::PairCoords() {
  return m_PairCoords;
}
