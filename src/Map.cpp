/// Implementation of Map
/**
 * @file Map.cpp
 */

#include <iostream>
#include <fstream>
#include "MyException.h"
#include "Map.h"
#include "Blank.h"
#include "BonusCoin.h"
#include "Coin.h"
#include "Ghost.h"
#include "Pacman.h"
#include "Portal.h"
#include "Wall.h"

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

void Map::Resize( const int & height, const int & width ) {
  // TODO
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

void Map::LoadFromFile( const std::string & path ) {
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
    GameObject * o;
    bool valid = false;
    if ( c == '-' ) {
      // coin
      o = new Coin();
      valid = true;
    }

    if ( c == '#' ) {
      // wall
      o = new Wall();
      if ( valid ) {
        throw MyException( std::string( "Invalid character '" ) + c + "' in map @ "
                           + std::to_string( row ) + "," + std::to_string( col ) );
      }
      valid = true;
    }

    if ( c == '*' ) {
      // bonus coin
      o = new BonusCoin();
      if ( valid ) {
        throw MyException( std::string( "Invalid character '" ) + c + "' in map @ "
                           + std::to_string( row ) + "," + std::to_string( col ) );
      }
      valid = true;
    }

    if ( c == ' ' ) {
      // blank
      o = new Blank();
      if ( valid ) {
        throw MyException( std::string( "Invalid character '" ) + c + "' in map @ "
                           + std::to_string( row ) + "," + std::to_string( col ) );
      }
      valid = true;
    }

    if ( c >= 'A' && c <= 'C' ) {
      // ghost
      o = new Ghost( c );
      if ( valid ) {
        throw MyException( std::string( "Invalid character '" ) + c + "' in map @ "
                           + std::to_string( row ) + "," + std::to_string( col ) );
      }
      valid = true;
    }

    if ( c >= '0' && c <= '9' ) {
      // portal
      o = new Portal( c - 48 );
      if ( valid ) {
        throw MyException( std::string( "Invalid character '" ) + c + "' in map @ "
                           + std::to_string( row ) + "," + std::to_string( col ) );
      }
      valid = true;
    }

    if ( c == 'P' ) {
      // pacman
      o = new Pacman();
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
    ( m_Data )[ row ].push_back( o );
    ++col;
  }

  {
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
    m_Width = ( --it )->size();
    m_Height = row;
  }
}
