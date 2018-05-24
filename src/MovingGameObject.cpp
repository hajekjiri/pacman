/// Implementation of MovingGameObject
/**
 * @file MovingGameObject.cpp
 */

#include <sstream>
#include <utility>
#include "BfsPathFinder.h"
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

const bool MovingGameObject::MovePacman( const int & direction, Game & game ) {
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

  // MovingGameObject is Pacman and new coords contain a ghost
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
        oss << "Ghost '" << game.GetMap().Data()[ newCoords.first ][ newCoords.second ]->Char()
            << "' not found in game data";
        throw MyException( oss.str() );
      }
      m_Carry = new GameObject( ' ', false );
      delete ( *it )->m_Carry;
      delete *it;
      game.Ghosts().erase( it );
      game.Score() += 5;
      game.GetMap().Data()[ newCoords.first ][ newCoords.second ] = new GameObject( ' ', false );
        m_Lethal = false;
      game.BonusTurns() = 0;
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

  switch ( m_Carry->Char() ) {
    case '-':
      ++game.Score();
      m_Carry->Char() = ' ';
      return true;
    case '*':
      game.BonusTurns() = atoi( game.Setting( "bonus_duration" ) ) + 1;
      game.Score() += 3;
      m_Carry->Char() = ' ';
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

  return true;
}

void MovingGameObject::MoveGhost( Game & game ) {
  BfsPathFinder pf( &( game.GetMap() ) );

  char direction = pf.GetFirstStep( m_Coords, game.Pacman()->Coords() );

  if ( direction != 'r' ) {
    std::ostringstream oss;
    oss << "Ghost/Path: " << m_Char << "/" << direction;
    //throw MyException( oss.str() );
  }

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
    case 'r':
      throw MyException( std::string( "TODO: random ghost direction" ) );
  }

  if ( ! game.GetMap().ValidCoords( newCoords ) ||
       game.GetMap().Data()[ newCoords.first ][ newCoords.second ]->Char() == '#' ) {
    std::ostringstream oss;
    oss << "Ghost '" << m_Char << "' attempted to move into invalid object '"
        << game.GetMap().Data()[ newCoords.first ][ newCoords.second ]->Char() << "'";
    throw MyException( oss.str() );
  }

  // update Pacman's coords, save old coords
  std::pair<int, int> oldCoords = m_Coords;
  m_Coords = newCoords;


  // save old carrry
  GameObject * tmp = m_Carry;

  // get new carry
  m_Carry = game.GetMap().Data()[ newCoords.first ][ newCoords.second ];

  // place ghost to new coords
  game.GetMap().Data()[ newCoords.first ][ newCoords.second ] = this;

  // place old carry to old coords
  game.GetMap().Data()[ oldCoords.first ][ oldCoords.second ] = tmp;

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
        return;
      }
    }
  }
  //
}
