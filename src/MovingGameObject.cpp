/// Implementation of MovingGameObject
/**
 * @file MovingGameObject.cpp
 */

#include <sstream>
#include <utility>
#include "Game.h"
#include "Map.h"
#include "MovingGameObject.h"
#include "MyException.h"

MovingGameObject::MovingGameObject( const char & c,
                                    const std::pair<int, int> & coords,
                                    const int & speed,
                                    const bool & lethal )
                                  : GameObject( c, lethal ),
                                    m_Coords( coords ),
                                    m_Alive( true ),
                                    m_Speed( speed ) {
  if ( m_Char < 'A' || m_Char > 'Z' ) {
    std::ostringstream oss;
    oss << "Invalid MovingGameObject with id '" << m_Char << "'";
    throw MyException( oss.str() );
  }

  if ( m_Char == 'P' ) {
    m_Carry = new GameObject( ' ', false );
  } else {
    m_Carry = new GameObject( '-', false );
  }
}

MovingGameObject::~MovingGameObject() {
  // do nothing
}

std::pair<int, int> & MovingGameObject::Coords() {
  return m_Coords;
}

bool & MovingGameObject::Alive() {
  return m_Alive;
}

int & MovingGameObject::Speed() {
  return m_Speed;
}

GameObject * MovingGameObject::Carry() {
  return m_Carry;
}

const bool MovingGameObject::Move( const int & direction, Game & game ) {
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

  if ( ! game.GetMap().ValidCoords( newCoords ) ||
       game.GetMap().Data()[ newCoords.first ][ newCoords.second ]->Char() == '#' ) {
    return false;
  }

  // update Pacman's coords, save old coords
  std::pair<int, int> oldCoords = m_Coords;
  m_Coords = newCoords;

  GameObject * tmp = m_Carry;

  // new coords contain a ghost
  if ( game.GetMap().Data()[ newCoords.first ][ newCoords.second ]->Char() >= 'A' &&
       game.GetMap().Data()[ newCoords.first ][ newCoords.second ]->Char() <= 'Z' ) {
    if ( m_Lethal ) {
      bool found = false;
      auto it = game.Ghosts().begin();
      for ( ;
            it != game.Ghosts().end();
            ++it ) {
        if ( ( *it )->Char() == game.GetMap().Data()[ newCoords.first ][ newCoords.second ]->Char() ) {
          found = true;
          break;
        }
      }
      if ( ! found ) {
        std::ostringstream oss;
        oss << "Ghost '" << game.GetMap().Data()[ newCoords.first ][ newCoords.second ]->Char() << "' not found in game data";
        throw MyException( oss.str() );
      }
      m_Carry = new GameObject( ' ', false );
      delete ( *it )->m_Carry;
      delete *it;
      game.Ghosts().erase( it );
      game.Score() += 5;
      game.GetMap().Data()[ newCoords.first ][ newCoords.second ] = new GameObject( ' ', false );
    } else {
      m_Carry = nullptr;
      m_Alive = false;
      return true;
    }
  } else {
    m_Carry = game.GetMap().Data()[ newCoords.first ][ newCoords.second ];
  }

  game.GetMap().Data()[ newCoords.first ][ newCoords.second ] = this;

  if ( tmp ) {
    game.GetMap().Data()[ oldCoords.first ][ oldCoords.second ] = tmp;
  }

  if ( ! m_Carry ) {
    return true;
  }

  if ( m_Char == 'P' ) {
    // MovingGameObject is Pacman
    switch ( m_Carry->Char() ) {
      case '-':
        ++game.Score();
        m_Carry->Char() = ' ';
        return true;
      case '*':
        // TODO: temporary power-up
        return true;
    }

    if ( m_Carry->Char() >= '0' && m_Carry->Char() <= '9' ) {
      // portal
      // find pair portal, move to its position, update carry
      for ( const auto & elem : game.Portals() ) {
        if ( m_Carry->Char() == elem->Id() &&
             m_Coords != elem->Coords() ) {
          oldCoords = m_Coords;
          m_Coords = elem->Coords();
          tmp = m_Carry;
          m_Carry = game.GetMap().Data()[ elem->Coords().first ][ elem->Coords().second ];
          // if carry is a moving object, set carry to nullptr
          if ( m_Carry->Char() >= 'A' && m_Carry->Char() <= 'Z' ) {
            m_Carry = nullptr;
          }
          game.GetMap().Data()[ m_Coords.first ][ m_Coords.second ] = this;
          if ( tmp ) {
            game.GetMap().Data()[ oldCoords.first ][ oldCoords.second ] = tmp;
          }
          return true;
        }
      }
    }
  }


  if ( m_Carry->Char() >= 'A' &&
       m_Carry->Char() <= 'Z' &&
       m_Carry->Char() != 'P' ) {
    // MovingGameObject is a ghost
    // TODO
  }

  return true;
}
