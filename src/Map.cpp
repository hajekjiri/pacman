/// Implementation of Map
/**
 * @file Map.cpp
 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "MyException.h"
#include "Map.h"
#include "Game.h"
#include "GameObject.h"
#include "MovingGameObject.h"

Map::Map() {
  // TODO
}

Map::~Map() {
  // do nothing
  // Map is deleted inside Game class for reasons, ask me why if curious
}

void Map::Draw( WINDOW * w ) {
  werase( w );
  int i = 0;
  int j = 0;
  for ( const auto & outsideElem : m_Data ) {
    for ( const auto & insideElem : outsideElem ) {
      std::ostringstream oss;
      oss << insideElem->Char();
      mvwprintw( w, i, j, oss.str().data() );
      ++j;
    }
    ++i;
    j = 0;
  }
}

std::vector<std::vector<GameObject*> > & Map::Data() {
  return m_Data;
}

const bool Map::ValidCoords( const std::pair<int, int> & coords ) const {
  if ( coords.first < 0 || coords.first >= m_Height ||
       coords.second < 0 || coords.second >= m_Width ) {
    return false;
  }
  return true;
}

std::vector<std::vector<GameObject*> >::iterator Map::CheckSize() {
  int row = 0;
  auto it = m_Data.begin();
  size_t size = it->size();
  for ( ; it != m_Data.end(); ++it, ++row ) {
    if ( it->size() != size ) {
      throw MyException( std::string( "Invalid map. Rows 0 and " )
                         + std::to_string( row )
                         + " are not the same length ( "
                         + std::to_string( size ) + " x "
                         + std::to_string( it->size() ) + " )" );
    }
  }
  return it;
}

void Map::LoadFromFile( const std::string & path, Game & game ) {
  std::ifstream is;
  is.open( ( "./src/cfg/" + path ).data() );
  if ( ! is ) {
    // map file does not exist
    throw MyException( std::string( "Map file '" )
                       + path + "' does not exist" );
  }
  char c;
  int rowIndex = 0;
  int col = 0;
  bool newLine = true;
  int rowAmt = 0;
  std::vector<Portal*> portals;
  bool pacmanExists = false;
  while ( ! is.eof() ) {
    is.get( c );
    if ( is.bad() ) {
      throw MyException( std::string( "Error while reading map from from file" ) );
    }
    if ( c == '\n' ) {
      ++rowIndex;
      col = 0;
      newLine = true;
      continue;
    }

    GameObject * o = nullptr;
    MovingGameObject * mo = nullptr;
    bool valid = false;

    if ( c == 'P' ) {
      pacmanExists = true;
      mo = new MovingGameObject( c, { rowIndex, col }, 1, false );
      valid = true;
      game.m_Pacman = mo;
    }

    if ( c >= 'A' && c <= 'C' ) {
      mo = new MovingGameObject( c, { rowIndex, col }, 1, true );
      if ( valid ) {
        throw MyException( std::string( "Invalid character '" ) + c + "' in map @ "
                           + std::to_string( rowIndex ) + "," + std::to_string( col ) );
      }
      valid = true;
      game.m_Ghosts.push_back( mo );
    }

    if ( c >= '0' && c <= '9' ) {
      o = new GameObject( c, false );
      if ( valid ) {
        throw MyException( std::string( "Invalid character '" ) + c + "' in map @ "
                           + std::to_string( rowIndex ) + "," + std::to_string( col ) );
      }
      valid = true;
      portals.push_back( new Portal( c, { rowIndex, col } ) );
    }

    if ( c == '*' ) {
      o = new GameObject( c, false );
      game.BonusCoords().push_back( { rowIndex, col } );
      if ( valid ) {
        throw MyException( std::string( "Invalid character '" ) + c + "' in map @ "
                           + std::to_string( rowIndex ) + "," + std::to_string( col ) );
      }
      valid = true;
    }

    if ( c == '-' || c == '#' || c == ' ' ) {
      o = new GameObject( c, false );
      if ( valid ) {
        throw MyException( std::string( "Invalid character '" ) + c + "' in map @ "
                           + std::to_string( rowIndex ) + "," + std::to_string( col ) );
      }
      valid = true;
    }

    if ( ! valid ) {
      throw MyException( std::string( "Invalid character '" ) + c + "' in map @ "
                         + std::to_string( rowIndex ) + "," + std::to_string( col ) );
    }

    if ( newLine ) {
      // increment row amount once an object is being pushed to the row
      ++rowAmt;
      m_Data.push_back( std::vector<GameObject*>() );
      newLine = false;
    }

    if ( o ) {
      m_Data[ rowIndex ].push_back( o );
    } else if ( mo ) {
      m_Data[ rowIndex ].push_back( mo );
    } else {
      throw MyException( std::string( "Invalid character '" ) + c + "' in map @ "
                         + std::to_string( rowIndex ) + "," + std::to_string( col ) );
    }

    ++col;
  }

  // map size is checked in the following m_Width assignment
  m_Width = ( --CheckSize() )->size();
  m_Height = rowAmt;

  if ( ! pacmanExists ) {
    throw MyException( std::string( "Invalid map - Pacman ( char 'P' )not found" ) );
  }

  // portals - check for duplicates
  for ( const auto & outsideElem : portals ) {
    int timesFound = 0;
    for ( const auto & insideElem : portals ) {
      if ( outsideElem->Id() == insideElem->Id() ) {
        ++timesFound;
      }
    }
    if ( timesFound != 2 ) {
      std::ostringstream oss;
      oss << "Invalid map - portal '" << outsideElem->Id()
          << "' was found " << timesFound << " times ( expecting 2 times )";
      throw MyException( oss.str() );
    }
  }

  // portals - pairing
  for ( const auto & outsideElem : portals ) {
    bool found = false;
    for ( const auto & insideElem : portals ) {
      if ( ( outsideElem->Id() == insideElem->Id() ) &&
           ( outsideElem->Coords() != insideElem->Coords() ) ) {
        outsideElem->PairCoords() = insideElem->Coords();
        insideElem->PairCoords() = outsideElem->Coords();
        found = true;
        break;
      }
    }
    if ( ! found ) {
      std::ostringstream oss;
      oss << "Portal pairing - pair for portal '" << outsideElem->Id()
          << "' was not found";
      // portals are not in Game class yet so we have to delete them manually
      for ( const auto & elem : portals ) {
        delete elem;
      }
      throw MyException( oss.str() );
    }
  }

  // portals - transfer to Game class
  for ( const auto & elem : portals ) {
    game.m_Portals.push_back( elem );
  }

  for ( const auto & outsideElem : game.Ghosts() ) {
    int times = 0;
    for ( const auto & insideElem : game.Ghosts() ) {
      if ( outsideElem->Char() == insideElem->Char() ) {
        ++times;
      }
    }
    if ( times != 1 ) {
      std::ostringstream oss;
      oss << "Invalid map - there are probably multiple instances of ghost '"
          << outsideElem->Char() << "'";
      throw MyException( oss.str() );
    }
  }
}
