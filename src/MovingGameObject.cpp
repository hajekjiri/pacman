/// Implementation of MovingGameObject
/**
 * @file MovingGameObject.cpp
 */

#include <utility>
#include "Map.h"
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

const bool MovingGameObject::Move( const int & direction, Map & map ) {
  std::pair<int, int> newCoords = m_Coords;
  switch ( direction ) {
    case 'w':
      --newCoords.first;
      break;
    case 'a':
      --newCoords.second;
      break;
    case 's':
      ++newCoords.first;
      break;
    case 'd':
      ++newCoords.second;
      break;
  }
  if ( ! map.ValidCoords( newCoords ) || map.Data()[ newCoords.first ][ newCoords.second ]->Char() == '#' ) {
    return false;
  }

  // update Pacman's coords, save old coords
  std::pair<int, int> oldCoords = m_Coords;
  m_Coords = newCoords;

  // place blank to old coords
  map.Data()[ oldCoords.first ][ oldCoords.second ] = new GameObject( ' ' );

  // delete what's on new coords
  delete map.Data()[ newCoords.first ][ newCoords.second ];

  // place Pacman to new coords
  map.Data()[ newCoords.first ][ newCoords.second ] = this;

  return true;
}
