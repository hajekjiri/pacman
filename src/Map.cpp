/// Implementation of Map
/**
 * @file Map.cpp
 */

 #include <iostream>
#include "Map.h"

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
