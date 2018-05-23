/// Implementation of Pacman
/**
 * @file Pacman.cpp
 */

#include "Pacman.h"
#include "Map.h"

Pacman::Pacman( const std::pair<int, int> & coords,
                const int & speed,
                const bool & lethal )
              : MovingGameObject( 'P', coords, speed, lethal ) {
  // TODO
}

Pacman::~Pacman() {
 // TODO
}

void Pacman::Interact( Ghost & o, Map & map ) {
  // TODO
}

void Pacman::Interact( Pacman & o, Map & map ) {
  // TODO
}

void Pacman::Move( const int & direction, Map & map ) {
  switch ( direction ) {
    case 'w':
      map.m_Data[ m_Coord.first + 1 ][ m_Coord.second ]->Interact( this, map );
      break;
    case 'a':
      map.m_Data[ m_Coord.first ][ m_Coord.second - 1 ]->Interact( this, map );
      break;
    case 's':
      map.m_Data[ m_Coord.first ][ m_Coord.second - 1 ]->Interact( this, map );
      break;
    case 'd':
      map.m_Data[ m_Coord.first ][ m_Coord.second + 1 ]->Interact( this, map );
      break;
  }
}
