/// Implementation of Map
/**
 * @file Map.cpp
 */

#include <iostream>
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
  for ( const auto & outsideElem : m_Data ) {
    for ( const auto & insideElem : outsideElem ) {
      delete insideElem;
    }
  }
}

void Map::Draw( WINDOW * w ) {
  int i = 0;
  int j = 0;
  for ( const auto & outsideElem : m_Data ) {
    for ( const auto & insideElem : outsideElem ) {
      mvwprintw( w, i, j, &( insideElem->Char() ) );
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
  int row = 0;
  int col = 0;
  bool newLine = true;
  while ( ! is.eof() ) {
    is.get( c );
    if ( is.bad() ) {
      throw MyException( std::string( "Error while reading map from from file" ) );
    }
    if ( c == '\n' ) {
      ++row;
      col = 0;
      newLine = true;
      continue;
    }

    GameObject * o = nullptr;
    MovingGameObject * mo = nullptr;
    bool valid = false;

    if ( c == 'P' ) {
      mo = new MovingGameObject( c, { row, col }, 1, false );
      valid = true;
      game.m_Pacman = mo;
    }

    if ( c >= 'A' && c <= 'C' ) {
      mo = new MovingGameObject( c, { row, col }, 1, true );
      if ( valid ) {
        throw MyException( std::string( "Invalid character '" ) + c + "' in map @ "
                           + std::to_string( row ) + "," + std::to_string( col ) );
      }
      valid = true;
      game.m_Ghosts.push_back( mo );
    }

    if ( c == '-' || c == '#' || c == '*' || c == ' ' || ( c >= '0' && c <= '9' ) ) {
      o = new GameObject( c );
      if ( valid ) {
        throw MyException( std::string( "Invalid character '" ) + c + "' in map @ "
                           + std::to_string( row ) + "," + std::to_string( col ) );
      }
      valid = true;
    }

    if ( ! valid ) {
      throw MyException( std::string( "Invalid character '" ) + c + "' in map @ "
                         + std::to_string( row ) + "," + std::to_string( col ) );
    }

    if ( newLine ) {
      m_Data.push_back( std::vector<GameObject*>() );
      newLine = false;
    }

    if ( o ) {
      m_Data[ row ].push_back( o );
    } else if ( mo ) {
      m_Data[ row ].push_back( mo );
    } else {
      throw MyException( std::string( "Invalid character '" ) + c + "' in map @ "
                         + std::to_string( row ) + "," + std::to_string( col ) );
    }

    ++col;
  }
  m_Width = ( --CheckSize() )->size();
  m_Height = row;
}
