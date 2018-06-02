/// Implementation of Map
/**
 * @file Map.cpp
 */

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <sys/ioctl.h>
#include "CommonFunctions.h"
#include "MyException.h"
#include "Map.h"
#include "Game.h"
#include "MovingGameObject.h"

const int Map::COLOR_PACMAN = 1;
const int Map::COLOR_GHOST_A = 2;
const int Map::COLOR_GHOST_B = 3;
const int Map::COLOR_GHOST_C = 4;
const int Map::COLOR_WALL = 5;
const int Map::COLOR_BLANK = 6;
const int Map::COLOR_COIN = 7;
const int Map::COLOR_BONUS_COIN = 8;
const int Map::COLOR_PORTAL = 9;

Map::Map() {
  // initialize color pairs
  init_pair( Map::COLOR_PACMAN, COLOR_YELLOW, COLOR_BLACK );
  init_pair( Map::COLOR_GHOST_A, COLOR_RED, COLOR_BLACK );
  init_pair( Map::COLOR_GHOST_B, COLOR_MAGENTA, COLOR_BLACK );
  init_pair( Map::COLOR_GHOST_C, COLOR_CYAN, COLOR_BLACK );
  init_pair( Map::COLOR_WALL, COLOR_BLACK, COLOR_WHITE );
  init_pair( Map::COLOR_BLANK, COLOR_BLACK, COLOR_BLACK );
  init_pair( Map::COLOR_COIN, COLOR_WHITE, COLOR_BLACK );
  init_pair( Map::COLOR_BONUS_COIN, COLOR_GREEN, COLOR_BLACK );
  init_pair( Map::COLOR_PORTAL, COLOR_BLUE, COLOR_WHITE );
}

Map::~Map() {
  /*
   * do nothing
   * Map is deleted in Game's destructor
   */
}

void Map::Draw( WINDOW * w ) {
  werase( w );
  int i = 0;
  int j = 0;
  for ( const auto & outsideElem : m_Data ) {
    for ( const auto & insideElem : outsideElem ) {
      std::ostringstream oss;
      oss << insideElem->GetChar();

      // print different objects with different colors
      switch( insideElem->GetChar() ) {
        case 'P':
          mvwprintw_color( w, i, j, oss.str().data(), Map::COLOR_PACMAN );
          break;
        case '*':
          mvwprintw_color( w, i, j, oss.str().data(), Map::COLOR_BONUS_COIN );
          break;
        case ' ':
          mvwprintw_color( w, i, j, oss.str().data(), Map::COLOR_BLANK );
          break;
        case '-':
          mvwprintw_color( w, i, j, oss.str().data(), Map::COLOR_COIN );
          break;
        case '#':
          mvwprintw_color( w, i, j, oss.str().data(), Map::COLOR_WALL );
          break;
        default:
          mvwprintw( w, i, j, oss.str().data() );
          break;
      }

      if ( isGhost( insideElem->GetChar() ) ) {
        int colorPairNo = ( ( insideElem->GetChar() - 'A' ) % 3 ) + Map::COLOR_GHOST_A;
        mvwprintw_color( w, i, j, oss.str().data(), colorPairNo );
      }
      if ( insideElem->GetChar() >= '0' &&
           insideElem->GetChar() <= '9' ) {
        mvwprintw_color( w, i, j, oss.str().data(), Map::COLOR_PORTAL );
      }
      ++j;
    }
    ++i;
    j = 0;
  }
  wrefresh( w );
}

std::vector<std::vector<GameObject*> > & Map::GetData() {
  return m_Data;
}

const int & Map::GetHeightConst() const {
  return m_Height;
}

const int & Map::GetWidthConst() const {
  return m_Width;
}

const bool Map::ValidCoords( const std::pair<int, int> & coords ) const {
  // check if coordinates are out of range
  if ( coords.first < 0 || coords.first >= m_Height ||
       coords.second < 0 || coords.second >= m_Width ) {
    return false;
  }
  return true;
}

std::vector<std::vector<GameObject*> >::iterator Map::CheckSize() {
  if ( m_Data.size() == 0 ) {
    throw MyException( "Invalid map - 0 rows" );
  }
  int row = 0;
  auto it = m_Data.begin();
  size_t size = it->size();
  auto itEnd = m_Data.end();
  // place end iterator past the last non-empty row
  while( ( --itEnd )->size() == 0 ) {
    // do nothing
  }
  ++itEnd;

  // check if all rows are the same length
  for ( ; it != itEnd; ++it, ++row ) {
    if ( it->size() != size ) {
      std::ostringstream oss;
      oss << "Invalid map. Rows 0 and " << row << " are not the same length ( " << size << " x " <<  it->size() << " )";
      throw MyException( oss.str() );
    }
  }

  // return iterator to last non-empty row
  return --it;
}

void Map::LoadFromFile( const std::string & path, Game & game ) {
  // delete all game objects
  if ( game.GetPacman() ) {
    delete game.GetPacman()->GetCarry();
  }
  for ( const auto & elem : game.GetGhosts() ) {
    delete elem->GetCarry();
  }
  game.GetGhosts().clear();

  for ( const auto & elem : game.GetPortals() ) {
    delete elem;
  }
  game.GetPortals().clear();

  game.GetBonusCoords().clear();

  for ( const auto & outsideElem : m_Data ) {
    for ( const auto & insideElem : outsideElem ) {
      delete insideElem;
    }
  }
  m_Data.clear();

  std::ifstream is;
  is.open( ( path ).data() );
  if ( ! is ) {
    std::ostringstream oss;
    oss << "Could not open map file '" << path << "'";
    throw MyException( oss.str() );
  }

  // read character
  char c;

  // row index for writing
  int rowIndex = 0;

  /*
   * column index for writing,
   *  also number of columns after we read the whole file
   */
  int col = 0;

  // flag
  bool newLine = true;

  // separate variable for amount of rows
  int rowAmt = 0;

  // special portals container
  std::vector<Portal*> portals;

  // another flag
  bool pacmanExists = false;
  while ( ! is.eof() ) {
    is.get( c );

    // check for read error
    if ( is.bad() ) {
      is.close();
      std::ostringstream oss;
      oss << "Error while reading map from from file '" << path << "'";
      throw MyException( oss.str() );
    }

    // handle newline
    if ( c == '\n' ) {
      ++rowIndex;
      col = 0;

      // set newline flag
      newLine = true;
      continue;
    }

    GameObject * o = nullptr;
    MovingGameObject * mo = nullptr;
    bool valid = false;

    if ( c == 'P' ) {
      // if Pacman already exists
      if ( pacmanExists ) {
        std::ostringstream oss;
        oss << "Invalid character '" << c
            << "' in map @ { " << rowIndex << ", " << col
            << " }\n(duplicate Pacman )";
        throw MyException( oss.str() );
      }
      pacmanExists = true;

      // create object
      mo = new MovingGameObject( c, { rowIndex, col }, nullptr, false );

      // set flag
      valid = true;

      // set game's pointer to Pacman
      game.GetPacman() = mo;
    }

    if ( c >= 'A' && c <= 'Z' && c != 'P' ) {
      std::pair<int, int> * home;
      // if ghost is of type B, set home coordinates
      if ( ( ( ( c - 'A' ) % 3 ) + 'A' ) == 'B' ) {
        home = new std::pair<int, int>( { rowIndex, col } );
      } else {
        home = nullptr;
      }
      mo = new MovingGameObject( c, { rowIndex, col }, home, true );
      /*
       * if somehow char c qualifies for more objects ( impossible ),
       *  throw an exception
       */
      if ( valid ) {
        is.close();
        std::ostringstream oss;
        oss << "Invalid character '" << c
            << "' in map @ { " << rowIndex << ", " << col << " }";
        throw MyException( oss.str() );
      }
      valid = true;

      // push ghost to game data
      game.GetGhosts().push_back( mo );
    }

    if ( c >= '0' && c <= '9' ) {
      o = new GameObject( c );
      if ( valid ) {
        is.close();
        std::ostringstream oss;
        oss << "Invalid character '" << c
            << "' in map @ { " << rowIndex << ", " << col << " }";
        throw MyException( oss.str() );
      }
      valid = true;
      portals.push_back( new Portal( c, { rowIndex, col } ) );
    }

    if ( c == '*' ) {
      o = new GameObject( c );
      game.GetBonusCoords().push_back( { rowIndex, col } );
      if ( valid ) {
        is.close();
        std::ostringstream oss;
        oss << "Invalid character '" << c
            << "' in map @ { " << rowIndex << ", " << col << " }";
        throw MyException( oss.str() );
      }
      valid = true;
    }

    if ( c == '-' || c == '#' || c == ' ' ) {
      o = new GameObject( c );
      if ( valid ) {
        is.close();
        std::ostringstream oss;
        oss << "Invalid character '" << c
            << "' in map @ { " << rowIndex << ", " << col << " }";
        throw MyException( oss.str() );
      }
      valid = true;
    }

    if ( ! valid ) {
      is.close();
      std::ostringstream oss;
      oss << "Invalid character '" << c
          << "' in map @ { " << rowIndex << ", " << col << " }";
      throw MyException( oss.str() );
    }

    if ( newLine ) {
      /*
       * increment row amount once an object is being pushed to the row
       * ( => do not count empty rows towards height )
       */
      ++rowAmt;

      // push empty row to map's data
      m_Data.push_back( std::vector<GameObject*>() );

      // unset newline flag
      newLine = false;
    }

    if ( o ) {
      m_Data[ rowIndex ].push_back( o );
    } else if ( mo ) {
      m_Data[ rowIndex ].push_back( mo );
    } else {
      is.close();
      std::ostringstream oss;
      oss << "Invalid character '" << c
          << "' in map @ { " << rowIndex << ", " << col << " }";
      throw MyException( oss.str() );
    }

    ++col;
  }

  // check map size, save iterator to last non-empty row
  auto itLast = CheckSize();

  // set width and height
  m_Width = itLast->size();
  m_Height = rowAmt;

  // check if Pacman exists
  if ( ! pacmanExists ) {
    is.close();
    throw MyException( "Invalid map - Pacman ( 'P' ) not found" );
  }

  // check for excessive portals
  for ( const auto & outsideElem : portals ) {
    int timesFound = 0;
    for ( const auto & insideElem : portals ) {
      if ( outsideElem->GetIdConst() == insideElem->GetIdConst() ) {
        ++timesFound;
      }
    }
    if ( timesFound != 2 ) {
      std::ostringstream oss;
      oss << "Invalid map - portal '" << outsideElem->GetIdConst()
          << "' was found " << timesFound << " times ( expecting 2 times )";
      is.close();
      throw MyException( oss.str() );
    }
  }

  // find and setup pair portals
  for ( const auto & outsideElem : portals ) {
    bool found = false;
    for ( const auto & insideElem : portals ) {
      if ( ( outsideElem->GetIdConst() == insideElem->GetIdConst() ) &&
           ( outsideElem->GetCoordsConst() != insideElem->GetCoordsConst() ) ) {
        outsideElem->GetPairCoords() = insideElem->GetCoordsConst();
        insideElem->GetPairCoords() = outsideElem->GetCoordsConst();
        found = true;
        break;
      }
    }
    // another impossible check
    if ( ! found ) {
      std::ostringstream oss;
      oss << "Portal pairing - pair for portal '" << outsideElem->GetIdConst()
          << "' was not found";
      // portals are not in Game class yet so we have to delete them manually
      for ( const auto & elem : portals ) {
        delete elem;
      }
      is.close();
      throw MyException( oss.str() );
    }
  }

  // portals - transfer to Game class
  for ( const auto & elem : portals ) {
    game.GetPortals().push_back( elem );
  }

  // check for duplicate ghosts
  for ( const auto & outsideElem : game.GetGhosts() ) {
    int times = 0;
    for ( const auto & insideElem : game.GetGhosts() ) {
      if ( outsideElem->GetChar() == insideElem->GetChar() ) {
        ++times;
      }
    }
    if ( times != 1 ) {
      std::ostringstream oss;
      oss << "Invalid map - there are multiple instances of ghost '"
          << outsideElem->GetChar() << "'";
      is.close();
      throw MyException( oss.str() );
    }
  }

  /*
   * sort ghosts by character
   * => this makes the ghosts move in order
   *    ( ghosts with lower characters move first )
   */
  std::sort( game.GetGhosts().begin(),
             game.GetGhosts().end(),
             []( MovingGameObject * const lhs, MovingGameObject * const rhs ) {
               return lhs->GetChar() < rhs->GetChar();
             } );
  is.close();

  // check if terminal window is big enough
  int minWidth = m_Width + 60;
  int minHeight = m_Height + 11;
  winsize size;
  ioctl( 0, TIOCGWINSZ, &size );
  if ( size.ws_row < minHeight || size.ws_col < minWidth ) {
    std::ostringstream oss;
    oss << "Your terminal window is too small.\nPlease resize it to at least "
        << minWidth << "x" << minHeight
        << " ( WxH ) and launch the game again";
    throw MyException( oss.str() );
  }
}
