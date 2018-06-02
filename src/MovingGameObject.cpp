/// Implementation of MovingGameObject
/**
 * @file MovingGameObject.cpp
 */

#include <climits>
#include <sstream>
#include "BfsPathFinder.h"
#include "CommonFunctions.h"
#include "MovingGameObject.h"
#include "MyException.h"

MovingGameObject::MovingGameObject( const char & c,
                                    const std::pair<int, int> & coords,
                                    std::pair<int, int> * const homeCoords,
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

std::pair<int, int> & MovingGameObject::GetCoords() {
  return m_Coords;
}

bool & MovingGameObject::GetAlive() {
  return m_Alive;
}

bool & MovingGameObject::GetLethal() {
  return m_Lethal;
}

GameObject *& MovingGameObject::GetCarry() {
  return m_Carry;
}

const bool MovingGameObject::MovePacman( const int & direction, Game & game ) {
  std::pair<int, int> newCoords = m_Coords;
  if ( direction == 'w' || direction == KEY_UP || direction == 'k' ) {
    --newCoords.first;
  }
  if ( direction == 'a' || direction == KEY_LEFT || direction == 'h' ) {
    --newCoords.second;
  }
  if ( direction == 's' || direction == KEY_DOWN || direction == 'j' ) {
    ++newCoords.first;
  }
  if ( direction == 'd' || direction == KEY_RIGHT || direction == 'l' ) {
    ++newCoords.second;
  }

  if ( ! game.GetMap().ValidCoords( newCoords ) ||
       game.GetMap().GetData()[ newCoords.first ][ newCoords.second ]->GetChar() == '#' ) {
    return false;
  }

  if ( game.GetTurnsConst() + 1 == game.GetRespawnBonusTurnNo() ) {
    game.RespawnBonus();
  }

  // update Pacman's coords, save old coords
  std::pair<int, int> oldCoords = m_Coords;
  if ( game.GetMap().GetData()[ newCoords.first ][ newCoords.second ]->GetChar() >= '0' &&
       game.GetMap().GetData()[ newCoords.first ][ newCoords.second ]->GetChar() <= '9' ) {
    bool found = false;
    for ( const auto & elem : game.GetPortals() ) {
      if ( game.GetMap().GetData()[ newCoords.first ][ newCoords.second ]->GetChar() == elem->GetIdConst() &&
           newCoords == elem->GetCoordsConst() ) {
        found = true;
        if ( isGhost( game.GetMap().GetData()[ elem->GetPairCoords().first ][ elem->GetPairCoords().second ]->GetChar() ) ) {
          return false;
        }
        break;
      }
    }
    if ( ! found ) {
      std::ostringstream oss;
      oss << "Portal '" << game.GetMap().GetData()[ newCoords.first ][ newCoords.second ]->GetChar()
          << "' not found in game data";
      throw MyException( oss.str().data() );
    }
  }


  m_Coords = newCoords;

  GameObject * tmp = m_Carry;

  // MovingGameObject is Pacman and new coords contain a ghost
  if ( isGhost( game.GetMap().GetData()[ newCoords.first ][ newCoords.second ]->GetChar() ) ) {
    bool found = false;
    auto it = game.GetGhosts().begin();
    for ( ;
          it != game.GetGhosts().end();
          ++it ) {
      if ( ( *it )->GetChar() == game.GetMap().GetData()[ newCoords.first ][ newCoords.second ]->GetChar() ) {
        found = true;
        break;
      }
    }
    if ( ! found ) {
      std::ostringstream oss;
      oss << "Ghost '" << game.GetMap().GetData()[ newCoords.first ][ newCoords.second ]->GetChar()
          << "' not found in game data";
      throw MyException( oss.str() );
    }

    if ( m_Lethal ) {
      m_Carry = ( *it )->GetCarry();
      delete *it;
      game.GetGhosts().erase( it );
      game.GetScore() += 5;
      m_Lethal = false;
      game.GetBonusTurns() = 0;
    } else {
      if ( m_Carry ) {
        game.GetMap().GetData()[ oldCoords.first ][ oldCoords.second ] = m_Carry;
      }
      m_Alive = false;
      m_Carry = nullptr;
      return true;
    }
  } else {
    m_Carry = game.GetMap().GetData()[ newCoords.first ][ newCoords.second ];
  }

  game.GetMap().GetData()[ newCoords.first ][ newCoords.second ] = this;

  if ( tmp ) {
    game.GetMap().GetData()[ oldCoords.first ][ oldCoords.second ] = tmp;
  }

  if ( ! m_Carry ) {
    return true;
  }

  switch ( m_Carry->GetChar() ) {
    case '-':
      ++game.GetScore();
      m_Carry->GetChar() = ' ';
      return true;
    case '*':
      game.GetBonusTurns() = game.FindSetting( "bonus_duration" ).GetIntConst() + 1;
      if ( game.GetRespawnBonusTurnNo() <= game.GetTurnsConst() + 1 ) {
        game.GetRespawnBonusTurnNo() = game.GetTurnsConst() + game.FindSetting( "bonus_interval" ).GetIntConst() + 1;
      }
      game.GetScore() += 3;
      m_Carry->GetChar() = ' ';
      return true;
  }

  if ( m_Carry->GetChar() >= '0' && m_Carry->GetChar() <= '9' ) {
    // portal
    // find pair portal, move to its position, update carry
    for ( const auto & elem : game.GetPortals() ) {
      if ( m_Carry->GetChar() == elem->GetIdConst() &&
           m_Coords != elem->GetCoordsConst() ) {
        oldCoords = m_Coords;
        m_Coords = elem->GetCoordsConst();
        tmp = m_Carry;
        m_Carry = game.GetMap().GetData()[ elem->GetCoordsConst().first ][ elem->GetCoordsConst().second ];
        game.GetMap().GetData()[ m_Coords.first ][ m_Coords.second ] = this;
        if ( tmp ) {
          game.GetMap().GetData()[ oldCoords.first ][ oldCoords.second ] = tmp;
        }
        return true;
      }
    }
  }

  return true;
}

void MovingGameObject::MoveGhost( Game & game ) {
  bool usePortals = game.FindSetting( "ghosts_portals_allowed" ).GetBoolConst();

  BfsPathFinder pf( &game, usePortals, false );
  std::pair<char, int> path = { 'f', -1 };
  switch ( ( ( m_Char - 'A' ) % 3 ) + 'A'  ) {
    case 'A': {
      path = pf.GetFirstStep( m_Coords, game.GetPacman()->GetCoords() );
      break;
    }

    case 'B': {
      if ( ! m_HomeCoords ) {
        std::ostringstream oss;
        oss << "Ghost '" << m_Char << "' doesnt have home coords";
        throw MyException( oss.str() );
      }

      auto subPath = pf.GetFirstStep( *m_HomeCoords, game.GetPacman()->GetCoords(), true );

      // if pacman has no path to ghost, dont move
      if ( subPath.first == 'n' &&
           game.GetPacman()->GetCoords() != *m_HomeCoords ) {
        path = subPath;
        throw MyException( std::string( "Pacman has no path to ghost" ) );
        break;
      }

      // if player is closer than X, chase pacman
      int distance = subPath.second;
      std::ostringstream oss;
      if ( distance < game.FindSetting( "ghost_aggressive_range" ).GetIntConst() ) {
        path = pf.GetFirstStep( m_Coords, game.GetPacman()->GetCoords() );
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
      for ( const auto & elem : game.GetBonusCoords() ) {
        if ( game.GetMap().GetData()[ elem.first ][ elem.second ]->GetChar() == '*' ) {
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
          path = pf.GetFirstStep( m_Coords, game.GetPacman()->GetCoords() );
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
       game.GetMap().GetData()[ newCoords.first ][ newCoords.second ]->GetChar() == '#' ||
        isGhost( game.GetMap().GetData()[ newCoords.first ][ newCoords.second ]->GetChar() ) ) {
    std::ostringstream oss;
    oss << "Ghost '" << m_Char << "' attempted to move into forbidden object '"
        << game.GetMap().GetData()[ newCoords.first ][ newCoords.second ]->GetChar() << "'";
    throw MyException( oss.str() );
  }

  // update ghost's coords, save old coords
  std::pair<int, int> oldCoords = m_Coords;
  m_Coords = newCoords;

  // MovingGameObject is a ghost and new coords contain Pacman
  if ( game.GetMap().GetData()[ newCoords.first ][ newCoords.second ]->GetChar() == 'P' ) {
    if ( game.GetPacman()->GetLethal() ) {
      bool found = false;
      auto it = game.GetGhosts().begin();
      for ( ;
            it != game.GetGhosts().end();
            ++it ) {
        if ( ( *it )->GetChar() == m_Char ) {
          found = true;
          break;
        }
      }
      if ( ! found ) {
        std::ostringstream oss;
        oss << "Ghost '" << game.GetMap().GetData()[ newCoords.first ][ newCoords.second ]->GetChar()
            << "' not found in game data";
        throw MyException( oss.str() );
      }
      m_Char = ' ';
      game.GetGhosts().erase( it );
      game.GetScore() += 5;
      game.GetPacman()->GetLethal() = false;
      game.GetBonusTurns() = 0;
      game.GetMap().GetData()[ oldCoords.first ][ oldCoords.second ] = m_Carry;
      delete this;
      return;
    } else {
      game.GetPacman()->GetAlive() = false;
      // place ghost to new coords
      game.GetMap().GetData()[ newCoords.first ][ newCoords.second ] = this;
      // place carry to old coords
      game.GetMap().GetData()[ oldCoords.first ][ oldCoords.second ] = m_Carry;
      m_Carry = game.GetPacman()->GetCarry();
      game.GetPacman()->GetCarry() = nullptr;
      return;
    }
  }

  // save old carrry
  GameObject * tmp = m_Carry;

  // get new carry
  m_Carry = game.GetMap().GetData()[ newCoords.first ][ newCoords.second ];

  // place ghost to new coords
  game.GetMap().GetData()[ newCoords.first ][ newCoords.second ] = this;

  // place old carry to old coords
  game.GetMap().GetData()[ oldCoords.first ][ oldCoords.second ] = tmp;

  if ( game.GetPacman()->GetAlive() == false ) {
    return;
  }

  if ( m_Carry && m_Carry->GetChar() == '*' ) {
    if ( game.GetRespawnBonusTurnNo() <= game.GetTurnsConst() ) {
      game.GetRespawnBonusTurnNo() = game.GetTurnsConst() + game.FindSetting( "bonus_interval" ).GetIntConst();
    }
      m_Carry->GetChar() = ' ';
      return;
  }

  if ( m_Carry->GetChar() >= '0' && m_Carry->GetChar() <= '9' ) {
    // portal
    // find pair portal, move to its position, update carry
    for ( const auto & elem : game.GetPortals() ) {
      if ( m_Carry->GetChar() == elem->GetIdConst() &&
           m_Coords != elem->GetCoordsConst() ) {
        if ( game.GetMap().GetData()[ elem->GetCoordsConst().first ][ elem->GetCoordsConst().second ]->GetChar() < '0' ||
             game.GetMap().GetData()[ elem->GetCoordsConst().first ][ elem->GetCoordsConst().second ]->GetChar() > '9') {
          std::ostringstream oss;
          oss << "Ghost '" << m_Char << "' attempted to go through portal '" << elem->GetIdConst()
              << "' while some other moving object was at the other end";
          throw MyException( oss.str().data() );
        }
        oldCoords = m_Coords;
        m_Coords = elem->GetCoordsConst();
        tmp = m_Carry;
        m_Carry = game.GetMap().GetData()[ elem->GetCoordsConst().first ][ elem->GetCoordsConst().second ];
        game.GetMap().GetData()[ m_Coords.first ][ m_Coords.second ] = this;
        if ( tmp ) {
          game.GetMap().GetData()[ oldCoords.first ][ oldCoords.second ] = tmp;
        }
        return;
      }
    }
  }
}
