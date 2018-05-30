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
  if ( game.GetMap().Data()[ newCoords.first ][ newCoords.second ]->Char() >= '0' &&
       game.GetMap().Data()[ newCoords.first ][ newCoords.second ]->Char() <= '9' ) {
    bool found = false;
    for ( const auto & elem : game.Portals() ) {
      if ( game.GetMap().Data()[ newCoords.first ][ newCoords.second ]->Char() == elem->Id() &&
           newCoords == elem->Coords() ) {
        found = true;
        if ( isGhost( game.GetMap().Data()[ elem->PairCoords().first ][ elem->PairCoords().second ]->Char() ) ) {
          return false;
        }
        break;
      }
    }
    if ( ! found ) {
      std::ostringstream oss;
      oss << "Portal '" << game.GetMap().Data()[ newCoords.first ][ newCoords.second ]->Char() << "' not found in game data";
      throw MyException( oss.str().data() );
    }
  }


  m_Coords = newCoords;

  GameObject * tmp = m_Carry;

  // MovingGameObject is Pacman and new coords contain a ghost
  if ( isGhost( game.GetMap().Data()[ newCoords.first ][ newCoords.second ]->Char() ) ) {
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
      game.BonusTurns() = game.GetSetting( "bonus_duration" ).GetInt() + 1;
      if ( game.RespawnBonusTurnNo() <= game.Turns() + 1 ) {
        game.RespawnBonusTurnNo() = game.Turns() + game.GetSetting( "bonus_interval" ).GetInt() + 1;
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
  bool usePortals = game.GetSetting( "ghosts_portals_allowed" ).GetBool();

  BfsPathFinder pf( &game, usePortals, false );
  std::pair<char, int> path = { 'f', -1 };
  switch ( ( ( m_Char - 'A' ) % 3 ) + 'A'  ) {
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
      if ( distance < game.GetSetting( "ghost_aggressive_range" ).GetInt() ) {
        path = pf.GetFirstStep( m_Coords, game.Pacman()->Coords() );
      } else {
        path = pf.GetFirstStep( m_Coords, *m_HomeCoords );
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
      std::ostringstream oss;
      oss << "Strategy deduction failed for ghost '" << m_Char << "'";
      throw MyException( oss.str().data() );
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
        isGhost( game.GetMap().Data()[ newCoords.first ][ newCoords.second ]->Char() ) ) {
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
    if ( game.RespawnBonusTurnNo() <= game.Turns() + 1 ) {
      game.RespawnBonusTurnNo() = game.Turns() + game.GetSetting( "bonus_interval" ).GetInt();
    }
      m_Carry->Char() = ' ';
      return;
  }

  if ( m_Carry->Char() >= '0' && m_Carry->Char() <= '9' ) {
    // portal
    // find pair portal, move to its position, update carry
    for ( const auto & elem : game.Portals() ) {
      if ( m_Carry->Char() == elem->Id() &&
           m_Coords != elem->Coords() ) {
        if ( game.GetMap().Data()[ elem->Coords().first ][ elem->Coords().second ]->Char() < '0' ||
             game.GetMap().Data()[ elem->Coords().first ][ elem->Coords().second ]->Char() > '9') {
          std::ostringstream oss;
          oss << "Ghost '" << m_Char << "' attempted to go through portal '" << elem->Id()
              << "' while some other moving object was on the other end";
          throw MyException( oss.str().data() );
        }
        oldCoords = m_Coords;
        m_Coords = elem->Coords();


/*
        if ( game.GetMap().Data()[ elem->Coords().first ][ elem->Coords().second ]->Char() == 'P' ) {
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
              oss << "Ghost '" << game.GetMap().Data()[ elem->Coords().first ][ elem->Coords().second ]->Char()
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
            game.GetMap().Data()[ elem->Coords().first ][ elem->Coords().second ] = this;
            // place carry to old coords
            game.GetMap().Data()[ oldCoords.first ][ oldCoords.second ] = m_Carry;
            m_Carry = game.Pacman()->m_Carry;
            game.Pacman()->m_Carry = nullptr;
            return;
          }
        }
*/
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
