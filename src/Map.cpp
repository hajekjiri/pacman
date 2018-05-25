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

const int & Map::Height() const {
  return m_Height;
}

const int & Map::Width() const {
  return m_Width;
}

const bool Map::ValidCoords( const std::pair<int, int> & coords ) const {
  if ( coords.first < 0 || coords.first >= m_Height ||
       coords.second < 0 || coords.second >= m_Width ) {
    return false;
  }
  return true;
}

std::vector<std::vector<GameObject*> >::iterator Map::CheckSize() {
  if ( m_Data.size() == 0 ) {
    throw MyException( std::string( "Invalid map - 0 rows" ) );
  }
  int row = 0;
  auto it = m_Data.begin();
  size_t size = it->size();
  auto itEnd = m_Data.end();
  while( ( --itEnd )->size() == 0 );
  ++itEnd;
  for ( ; it != itEnd; ++it, ++row ) {
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
  if ( game.Pacman() ) {
    delete game.Pacman()->Carry();
  }
  for ( const auto & elem : game.Ghosts() ) {
    delete elem->Carry();
  }
  game.Ghosts().clear();
  for ( const auto & elem : game.Portals() ) {
    delete elem;
  }
  game.Portals().clear();
  game.BonusCoords().clear();

  for ( const auto & outsideElem : m_Data ) {
    for ( const auto & insideElem : outsideElem ) {
      delete insideElem;
    }
  }
  m_Data.clear();

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
      is.close();
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
      mo = new MovingGameObject( c, { rowIndex, col }, nullptr, false );
      valid = true;
      game.Pacman() = mo;
    }

    if ( c >= 'A' && c <= 'C' ) {
      std::pair<int, int> * home;
      switch ( c ) {
        case 'A':
          home = nullptr;
          break;
        case 'B':
          home = new std::pair<int, int>( { rowIndex, col } );
          break;
        case 'C':
          home = nullptr;
          break;
        default:
          home = nullptr;
          break;
      }
      mo = new MovingGameObject( c, { rowIndex, col }, home, true );
      if ( valid ) {
        is.close();
        throw MyException( std::string( "Invalid character '" ) + c + "' in map @ "
                           + std::to_string( rowIndex ) + "," + std::to_string( col ) );
      }
      valid = true;
      game.Ghosts().push_back( mo );
    }

    if ( c >= '0' && c <= '9' ) {
      o = new GameObject( c );
      if ( valid ) {
        is.close();
        throw MyException( std::string( "Invalid character '" ) + c + "' in map @ "
                           + std::to_string( rowIndex ) + "," + std::to_string( col ) );
      }
      valid = true;
      portals.push_back( new Portal( c, { rowIndex, col } ) );
    }

    if ( c == '*' ) {
      o = new GameObject( c );
      game.BonusCoords().push_back( { rowIndex, col } );
      if ( valid ) {
        is.close();
        throw MyException( std::string( "Invalid character '" ) + c + "' in map @ "
                           + std::to_string( rowIndex ) + "," + std::to_string( col ) );
      }
      valid = true;
    }

    if ( c == '-' || c == '#' || c == ' ' ) {
      o = new GameObject( c );
      if ( valid ) {
        is.close();
        throw MyException( std::string( "Invalid character '" ) + c + "' in map @ "
                           + std::to_string( rowIndex ) + "," + std::to_string( col ) );
      }
      valid = true;
    }

    if ( ! valid ) {
      is.close();
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
      is.close();
      throw MyException( std::string( "Invalid character '" ) + c + "' in map @ "
                         + std::to_string( rowIndex ) + "," + std::to_string( col ) );
    }

    ++col;
  }

  // map size is checked in the following m_Width assignment
  m_Width = ( --CheckSize() )->size();
  m_Height = rowAmt;

  if ( ! pacmanExists ) {
    is.close();
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
      is.close();
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
      is.close();
      throw MyException( oss.str() );
    }
  }

  // portals - transfer to Game class
  for ( const auto & elem : portals ) {
    game.Portals().push_back( elem );
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
      oss << "Invalid map - there are multiple instances of ghost '"
          << outsideElem->Char() << "'";
      is.close();
      throw MyException( oss.str() );
    }
  }
  is.close();
}
