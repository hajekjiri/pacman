/// Implementation of Coin
/**
 * @file Coin.cpp
 */

#include "Coin.h"
#include "Map.h"
#include "Pacman.h"
#include "Ghost.h"
#include "Blank.h"

Coin::Coin( const std::pair<int, int> & coords )
          : SolidGameObject( '-', coords ) {
  // TODO
}

Coin::~Coin() {
  // TODO
}

void Coin::Interact( Ghost * o, Map & map ) {
  // TODO
}

void Coin::Interact( Pacman * o, Map & map ) {
  auto pacmanCoords = o->m_Coords;

  // update Pacman's coords
  o->m_Coords = m_Coords;
  map.m_Data[ m_Coords.first ][ m_Coords.second ] = o;

  // place a Blank object on Pacman's former position
  map.m_Data[ pacmanCoords.first ][ pacmanCoords.second ] = new Blank( pacmanCoords );

  delete this;
}
