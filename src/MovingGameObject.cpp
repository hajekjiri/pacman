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

  // ghosts will start out with a coin as their carry object
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
  // get new coordinates
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

  // determine if Pacman can move to new coordinates
  if ( ! game.GetMap().ValidCoords( newCoords ) ||
       game.GetMap().GetData()[ newCoords.first ][ newCoords.second ]->GetChar() == '#' ) {
    return false;
  }

  /*
   * if bonus coins are supposed to respawn in this turn,
   *  respawn them
   */
  if ( game.GetTurnsConst() + 1 == game.GetRespawnBonusTurnNo() ) {
    game.RespawnBonus();
  }

  // save old coordinates
  std::pair<int, int> oldCoords = m_Coords;

  if ( game.GetMap().GetData()[ newCoords.first ][ newCoords.second ]->GetChar() >= '0' &&
       game.GetMap().GetData()[ newCoords.first ][ newCoords.second ]->GetChar() <= '9' ) {
    /* find portal in game data,
     *  determine if Pacman can use it
     */
    bool found = false;
    for ( const auto & elem : game.GetPortals() ) {
      if ( game.GetMap().GetData()[ newCoords.first ][ newCoords.second ]->GetChar() == elem->GetIdConst() &&
           newCoords == elem->GetCoordsConst() ) {
        found = true;
        /*
         * if a ghost is at the other end of the portal,
         *  Pacman cannot go through
         */
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
      throw MyException( oss.str() );
    }
  }

  // update coordinates
  m_Coords = newCoords;

  // save old carry
  GameObject * tmp = m_Carry;

  // Pacman ran into a ghost
  if ( isGhost( game.GetMap().GetData()[ newCoords.first ][ newCoords.second ]->GetChar() ) ) {
    // find ghost in game data
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

    // if pacman is lethal
    if ( m_Lethal ) {
      // Pacman takes ghost's carry
      m_Carry = ( *it )->GetCarry();

      // delete ghost and remove it from game data
      delete *it;
      game.GetGhosts().erase( it );

      // add score
      game.GetScore() += 5;

      // make Pacman vulnerable again
      m_Lethal = false;

      // set amount of turns Pacman is lethal to 0
      game.GetBonusTurns() = 0;
    } else {
      // place carry to old coordinates
      game.GetMap().GetData()[ oldCoords.first ][ oldCoords.second ] = m_Carry;

      // die
      m_Alive = false;

      // unset carry
      m_Carry = nullptr;
      return true;
    }
  } else {
    // take carry from new coordinates
    m_Carry = game.GetMap().GetData()[ newCoords.first ][ newCoords.second ];
  }

  // place Pacman to new coordinates
  game.GetMap().GetData()[ newCoords.first ][ newCoords.second ] = this;

  // place old carry to old coordinates
  game.GetMap().GetData()[ oldCoords.first ][ oldCoords.second ] = tmp;

  switch ( m_Carry->GetChar() ) {
    case '-':
      // coin
      // +1 score
      ++game.GetScore();

      // replace with blank
      m_Carry->GetChar() = ' ';
      return true;
    case '*':
      /*
       * set amount of turns Pacman is lethal
       *  to <bonus_duration> from settings
       */
      game.GetBonusTurns() = game.FindSetting( "bonus_duration" ).GetIntConst() + 1;

      /*
       * if it's the first bonus coin being
       *  picked up from the current set,
       * set respawn turn number
       */
      if ( game.GetRespawnBonusTurnNo() <= game.GetTurnsConst() + 1 ) {
        game.GetRespawnBonusTurnNo() = game.GetTurnsConst() + game.FindSetting( "bonus_interval" ).GetIntConst() + 1;
      }

      // replace with blank
      m_Carry->GetChar() = ' ';
      return true;
  }

  // here, Pacman actually uses the portal
  if ( m_Carry->GetChar() >= '0' && m_Carry->GetChar() <= '9' ) {
    // find pair portal
    for ( const auto & elem : game.GetPortals() ) {
      // look for portal with same id but different coordinates
      if ( m_Carry->GetChar() == elem->GetIdConst() &&
           m_Coords != elem->GetCoordsConst() ) {
        oldCoords = m_Coords;

        // place old carry to old coordinates
        tmp = m_Carry;
        game.GetMap().GetData()[ oldCoords.first ][ oldCoords.second ] = tmp;

        // get new carry
        m_Carry = game.GetMap().GetData()[ elem->GetCoordsConst().first ][ elem->GetCoordsConst().second ];

        // move to pair portal's position
        m_Coords = elem->GetCoordsConst();
        game.GetMap().GetData()[ m_Coords.first ][ m_Coords.second ] = this;
        return true;
      }
    }
  }

  return true;
}

void MovingGameObject::MoveGhost( Game & game ) {
  // check whether ghosts can use portals
  bool usePortals = game.FindSetting( "ghosts_portals_allowed" ).GetBoolConst();

  // create an instance of pathfinder with noblock off
  BfsPathFinder pf( &game, usePortals, false );
  std::pair<char, int> path = { 'f', -1 };
  switch ( ( ( m_Char - 'A' ) % 3 ) + 'A'  ) {
    case 'A': {
      // get path to Pacman
      path = pf.GetFirstStep( m_Coords, game.GetPacman()->GetCoords() );
      break;
    }

    case 'B': {
      if ( ! m_HomeCoords ) {
        std::ostringstream oss;
        oss << "Ghost '" << m_Char << "' doesnt have home coords";
        throw MyException( oss.str() );
      }

      // get path from Pacman to ghost's home with noblock on
      auto subPath = pf.GetFirstStep( *m_HomeCoords, game.GetPacman()->GetCoords(), true );

      if ( subPath.first == 'n' &&
           game.GetPacman()->GetCoords() != *m_HomeCoords ) {
        path = subPath;
        throw MyException( "Pacman has no path to ghost of type B" );
      }

      /*
       * if Pacman is closer than <ghost_aggressive_range>,
       *  get path to Pacman,
       *  otherwise get path to home coordinates
       */
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
       *  blocking paths to all bonus coins, in which case
       *  path finder won't find a path
       */
      bool bonusFound = false;

      // get path to closest spawned bonus coin
      for ( const auto & elem : game.GetBonusCoords() ) {
        if ( game.GetMap().GetData()[ elem.first ][ elem.second ]->GetChar() == '*' ) {
          bonusFound = true;
          auto subPath = pf.GetFirstStep( m_Coords, elem );
          if ( subPath.first != 'n' && subPath.second < path.second ) {
            path = subPath;
          }
        }
      }

      /*
       * if no bonus coins are present,
       *  get path to Pacman
       */
      if ( path.first == 'f' ) {
        if ( bonusFound ) {
          path.first = 'n';
        } else {
          path = pf.GetFirstStep( m_Coords, game.GetPacman()->GetCoords() );
        }
      }
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
    case 'f':
      std::ostringstream oss;
      oss << "Path deduction failed for ghost '" << m_Char << "'";
      throw MyException( oss.str() );
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

  /*
   * update ghost's coordinates,
   *  save old coordinates
   */
  std::pair<int, int> oldCoords = m_Coords;
  m_Coords = newCoords;

  // ghost ran into Pacman
  if ( game.GetMap().GetData()[ newCoords.first ][ newCoords.second ]->GetChar() == 'P' ) {
    // if Pacman is lethal
    if ( game.GetPacman()->GetLethal() ) {
      // find ghost in game data
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

      // replace ghost with blank
      m_Char = ' ';

      // remove ghost from game data
      game.GetGhosts().erase( it );

      // +5 score
      game.GetScore() += 5;

      // make Pacman vulnerable again
      game.GetPacman()->GetLethal() = false;

      // set amount of turns Pacman is lethal to 0
      game.GetBonusTurns() = 0;

      // place carry to old coordinates
      game.GetMap().GetData()[ oldCoords.first ][ oldCoords.second ] = m_Carry;

      /*
       * delete ghost this way,
       *  because he's not accessible from anywhere anymore
       */
      delete this;
    } else {
      // kill Pacman
      game.GetPacman()->GetAlive() = false;

      // place ghost to new coordinates
      game.GetMap().GetData()[ newCoords.first ][ newCoords.second ] = this;

      // place carry to old coordinates
      game.GetMap().GetData()[ oldCoords.first ][ oldCoords.second ] = m_Carry;

      // take Pacman's carry
      m_Carry = game.GetPacman()->GetCarry();

      // unset Pacman's carry
      game.GetPacman()->GetCarry() = nullptr;
    }
    return;
  }

  // save old carrry
  GameObject * tmp = m_Carry;

  // get new carry
  m_Carry = game.GetMap().GetData()[ newCoords.first ][ newCoords.second ];

  // place ghost to new coordinates
  game.GetMap().GetData()[ newCoords.first ][ newCoords.second ] = this;

  // place old carry to old coordinates
  game.GetMap().GetData()[ oldCoords.first ][ oldCoords.second ] = tmp;

  /*
   * if Pacman's dead,
   *  return now
   */
  if ( game.GetPacman()->GetAlive() == false ) {
    return;
  }


  if ( m_Carry && m_Carry->GetChar() == '*' ) {
    /*
     * if it's the first bonus coin being
     *  picked up from the current set,
     * set respawn turn number
     */
    if ( game.GetRespawnBonusTurnNo() <= game.GetTurnsConst() ) {
      game.GetRespawnBonusTurnNo() = game.GetTurnsConst() + game.FindSetting( "bonus_interval" ).GetIntConst();
    }

    // replace with blank
    m_Carry->GetChar() = ' ';
    return;
  }

  // move through portal
  if ( m_Carry->GetChar() >= '0' && m_Carry->GetChar() <= '9' ) {
    // find pair portal
    for ( const auto & elem : game.GetPortals() ) {
      // look for portal with same id but different coordinates
      if ( m_Carry->GetChar() == elem->GetIdConst() &&
           m_Coords != elem->GetCoordsConst() ) {
        if ( game.GetMap().GetData()[ elem->GetCoordsConst().first ][ elem->GetCoordsConst().second ]->GetChar() < '0' ||
             game.GetMap().GetData()[ elem->GetCoordsConst().first ][ elem->GetCoordsConst().second ]->GetChar() > '9') {
          std::ostringstream oss;
          oss << "Ghost '" << m_Char << "' attempted to go through portal '" << elem->GetIdConst()
              << "' while some other moving object was at the other end";
          throw MyException( oss.str() );
        }

        // update coordinates
        oldCoords = m_Coords;
        m_Coords = elem->GetCoordsConst();

        // place old carry to old coordinates
        tmp = m_Carry;
        game.GetMap().GetData()[ oldCoords.first ][ oldCoords.second ] = tmp;

        // get new carry
        m_Carry = game.GetMap().GetData()[ elem->GetCoordsConst().first ][ elem->GetCoordsConst().second ];

        // move to pair portal's position
        game.GetMap().GetData()[ m_Coords.first ][ m_Coords.second ] = this;
        return;
      }
    }
  }
}
