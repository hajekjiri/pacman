/// Implementation of CommonFunctions
/**
 * @file CommonFunctions.cpp
 */

#include <algorithm>
#include <fstream>
#include "CommonFunctions.h"
#include "MyException.h"

void strLTrim( std::string & s ) {
  // erase everything before first non-whitespace character
  s.erase( s.begin(), find_if( s.begin(), s.end(), []( const int & c ) {
    return ! std::isspace( c );
  } ) );
}

void strRTrim( std::string & s ) {
  // erase everything before after non-whitespace character
  s.erase( find_if( s.rbegin(), s.rend(), []( const int & c ) {
    return ! std::isspace( c );
  } ).base(), s.end() );
}

void strTrim( std::string & s ) {
  // erase excess whitespace from both sides
  strLTrim( s );
  strRTrim( s );
}

void dumpToFile( const char * const s, const char * const pathToFile ) {
  std::ofstream os;

  // open file in append mode
  os.open( pathToFile, std::ios_base::app );

  if ( ! os.is_open() ) {
    throw MyException( "Unable to open log file" );
  }

  // write to file
  os << s;
  os.close();
}

const bool isGhost( const char & c ) {
  if ( c >= 'A' && c <= 'Z' && c != 'P' ) {
    return true;
  }
  return false;
}

const bool isNextToInLine( const std::pair<int, int> & p1,
                           const std::pair<int, int> & p2 ) {
  std::pair<int, int> result;
  result.first = p1.first - p2.first;
  result.second = p1.second - p2.second;
  if ( ( result.first == 1 && result.second == 0 ) ||
       ( result.first == -1 && result.second == 0 ) ||
       ( result.first == 0 && result.second == 1 ) ||
       ( result.first == 0 && result.second == -1 ) ) {
    return true;
  }
  return false;
}

void mvwprintw_color( WINDOW * w,
                      const int & y,
                      const int & x,
                      const char * const s,
                      const int & color ) {
  wattron( w, COLOR_PAIR( color ) );
  mvwprintw( w, y, x, s );
  wattroff( w, COLOR_PAIR( color ) );
}
