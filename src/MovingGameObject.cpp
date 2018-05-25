/// Implementation of MovingGameObject
/**
 * @file MovingGameObject.cpp
 */

#include <climits>
#include <sstream>
#include <utility>
#include "BfsPathFinder.h"
#include "Game.h"
#include "Map.h"
#include "MovingGameObject.h"
#include "MyException.h"

MovingGameObject::MovingGameObject( const char & c,
                                    const std::pair<int, int> & coords,
                                    std::pair<int, int> * homeCoords,
                                    const bool & lethal )
                                  : GameObject( c ),
                                    m_Coords( coords ),
                                    m_HomeCoords( homeCoords ),
                                    m_Alive( true ),
                                    m_Lethal( lethal ) {
  if ( m_Char < 'A' || m_Char > 'Z' ) {
    std::ostringstream oss;
    oss << "Invalid MovingGameObject with id '" << m_Char << "'";
    throw MyException( oss.str() );
  }

  if ( m_Char == 'P' ) {
    m_Carry = new GameObject( ' ' );
  } else {
    m_Carry = new GameObject( '-' );
  }
}

MovingGameObject::~MovingGameObject() {
  delete m_HomeCoords;
}

std::pair<int, int> & MovingGameObject::Coords() {
  return m_Coords;
}

bool & MovingGameObject::Alive() {
  return m_Alive;
}

bool & MovingGameObject::Lethal() {
  return m_Lethal;
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

  if ( game.Turns() + 1 == game.RespawnBonusTurnNo() ) {
    game.RespawnBonus();
  }

  // update Pacman's coords, save old coords
  std::pair<int, int> oldCoords = m_Coords;
  m_Coords = newCoords;

/*
  if ( m_Carry && m_Carry->Char() == '*' ) {
    game.BonusTurns() = atoi( game.Setting( "bonus_duration" ) );
    game.RespawnBonusTurnNo() = game.Turns() + atoi( game.Setting( "bonus_interval" ) );
    game.Score() += 3;
    m_Carry->Char() = ' ';
  }
*/

  GameObject * tmp = m_Carry;

  // MovingGameObject is Pacman and new coords contain a ghost
  if ( game.GetMap().Data()[ newCoords.first ][ newCoords.second ]->Char() >= 'A' &&
       game.GetMap().Data()[ newCoords.first ][ newCoords.second ]->Char() <= 'Z' ) {
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

    if ( m_Lethal ) {
      m_Carry = ( *it )->m_Carry;
      delete *it;
      game.Ghosts().erase( it );
      game.Score() += 5;
      m_Lethal = false;
      game.BonusTurns() = 0;
    } else {
      if ( m_Carry ) {
        game.GetMap().Data()[ oldCoords.first ][ oldCoords.second ] = m_Carry;
      }
      m_Alive = false;
      m_Carry = nullptr;
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
      if ( game.RespawnBonusTurnNo() <= game.Turns() + 1 ) {
        game.RespawnBonusTurnNo() = game.Turns() + atoi( game.Setting( "bonus_interval" ) ) + 1;
      }
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
  std::string usePortalsStr = game.Setting( "ghosts_portals_allowed" );
  bool usePortals;
  if ( usePortalsStr == "yes" ) {
    usePortals = true;
  } else if ( usePortalsStr == "no" ) {
    usePortals = false;
  } else {
    throw MyException( std::string( "Invalid cfg - syntax error near 'ghosts_portals_allowed:'" ) );
  }

  switch ( m_Char ) {
    case 'B': {
      break;
    }
    case 'C': {

      break;
    }
    default: {
      if ( ! m_HomeCoords ) {

      } else {
        std::ostringstream oss;
        oss << "Ghost '" << m_Char << "' has home coords set.\nThis should not have happened";
        throw MyException( oss.str() );
      }
    }
  }

  BfsPathFinder pf( &( game.GetMap() ), usePortals, false );
  std::pair<char, int> path = { 'f', -1 };
  switch ( m_Char ) {
    case 'A': {
      path = pf.GetFirstStep( m_Coords, game.Pacman()->Coords() );
      break;
    }

    case 'B': {
      if ( ! m_HomeCoords ) {
        std::ostringstream oss;
        oss << "Ghost '" << m_Char << "' doesnt have home coords";
        throw MyException( oss.str() );
      }

      auto subPath = pf.GetFirstStep( *m_HomeCoords, game.Pacman()->m_Coords, true );

      // if pacman has no path to ghost, dont move
      if ( subPath.first == 'n' &&
           game.Pacman()->Coords() != *m_HomeCoords ) {
        path = subPath;
        throw MyException( std::string( "Pacman has no path to ghost" ) );
        break;
      }

      // if player is closer than X, chase pacman
      int distance = subPath.second;
      std::ostringstream oss;
      if ( distance < atoi( game.Setting( "ghost_aggressive_range" ) ) ) {
        path = pf.GetFirstStep( m_Coords, game.Pacman()->Coords() );
        oss << "path to pacman: { " << path.first << ", " << path.second << " }\n";
        dumpToFile( oss.str().data() );
      } else {
        path = pf.GetFirstStep( m_Coords, *m_HomeCoords );
        oss << "path home: { " << path.first << ", " << path.second << " }\n";
        dumpToFile( oss.str().data() );
      }
      break;
    }

    case 'C': {
      path.second = INT_MAX;

      /*
       * we need this flag because other ghosts might be
       *  blocking paths to all bonuses, in which case
       *  path finder won't find a path
       */
      bool bonusFound = false;

      // get path to closest spawned bonus
      for ( const auto & elem : game.BonusCoords() ) {
        if ( game.GetMap().Data()[ elem.first ][ elem.second ]->Char() == '*' ) {
          bonusFound = true;
          auto subPath = pf.GetFirstStep( m_Coords, elem );
          if ( subPath.first != 'n' && subPath.second < path.second ) {
            path = subPath;
          }
        }
      }

      // if no bonuses are present, get path to player
      if ( path.first == 'f' ) {
        if ( bonusFound ) {
          path.first = 'n';
        } else {
          path = pf.GetFirstStep( m_Coords, game.Pacman()->Coords() );
        }
      }
      break;
    }

    default: {
      path = pf.GetFirstStep( m_Coords, game.Pacman()->Coords() );
      break;
    }
  }

  std::pair<int, int> newCoords = m_Coords;
  switch ( path.first ) {
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
    case 'n':
      // path does not exist, dont move
      return;
      break;
    case 'f':
      std::ostringstream oss;
      oss << "Path deduction failed for ghost '" << m_Char << "'";
      throw MyException( std::string( oss.str() ) );
      break;
  }

  if ( ! game.GetMap().ValidCoords( newCoords ) ||
       game.GetMap().Data()[ newCoords.first ][ newCoords.second ]->Char() == '#' ||
       ( game.GetMap().Data()[ newCoords.first ][ newCoords.second ]->Char() >= 'A' &&
         game.GetMap().Data()[ newCoords.first ][ newCoords.second ]->Char() <= 'Z' &&
         game.GetMap().Data()[ newCoords.first ][ newCoords.second ]->Char() != 'P' ) ) {
    std::ostringstream oss;
    oss << "Ghost '" << m_Char << "' attempted to move into forbidden object '"
        << game.GetMap().Data()[ newCoords.first ][ newCoords.second ]->Char() << "'";
    throw MyException( oss.str() );
  }

  // update ghost's coords, save old coords
  std::pair<int, int> oldCoords = m_Coords;
  m_Coords = newCoords;

  // MovingGameObject is a ghost and new coords contain Pacman
  if ( game.GetMap().Data()[ newCoords.first ][ newCoords.second ]->Char() == 'P' ) {
    if ( game.Pacman()->Lethal() ) {
      bool found = false;
      auto it = game.Ghosts().begin();
      for ( ;
            it != game.Ghosts().end();
            ++it ) {
        if ( ( *it )->Char() == m_Char ) {
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
      m_Char = ' ';
      game.Ghosts().erase( it );
      game.Score() += 5;
      game.Pacman()->Lethal() = false;
      game.BonusTurns() = 0;
      game.GetMap().Data()[ oldCoords.first ][ oldCoords.second ] = m_Carry;
      delete this; // meh
      return;
    } else {
      game.Pacman()->Alive() = false;
      // place ghost to new coords
      game.GetMap().Data()[ newCoords.first ][ newCoords.second ] = this;
      // place carry to old coords
      game.GetMap().Data()[ oldCoords.first ][ oldCoords.second ] = m_Carry;
      m_Carry = game.Pacman()->m_Carry;
      game.Pacman()->m_Carry = nullptr;
      return;
    }
  }

  // save old carrry
  GameObject * tmp = m_Carry;

  // get new carry
  m_Carry = game.GetMap().Data()[ newCoords.first ][ newCoords.second ];

  // place ghost to new coords
  game.GetMap().Data()[ newCoords.first ][ newCoords.second ] = this;

  // place old carry to old coords
  game.GetMap().Data()[ oldCoords.first ][ oldCoords.second ] = tmp;

  if ( game.Pacman()->Alive() == false ) {
    return;
  }

  if ( m_Carry && m_Carry->Char() == '*' ) {
      game.RespawnBonusTurnNo() = game.Turns() + atoi( game.Setting( "bonus_interval" ) );
      m_Carry->Char() = ' ';
      return;
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
        game.GetMap().Data()[ m_Coords.first ][ m_Coords.second ] = this;
        if ( tmp ) {
          game.GetMap().Data()[ oldCoords.first ][ oldCoords.second ] = tmp;
        }
        return;
      }
    }
  }
}
