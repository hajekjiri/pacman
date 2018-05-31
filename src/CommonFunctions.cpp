/// Implementation of CommonFunctions
/**
 * @file CommonFunctions.cpp
 */

#include <algorithm>
#include <fstream>
#include "CommonFunctions.h"
#include "MyException.h"

void strLTrim( std::string & s ) {
  s.erase( s.begin(), find_if( s.begin(), s.end(), []( int c ) {
    return ! std::isspace( c );
  } ) );
}

void strRTrim( std::string & s ) {
  s.erase( find_if( s.rbegin(), s.rend(), []( int c ) {
    return ! std::isspace( c );
  } ).base(), s.end() );
}

void strTrim( std::string & s ) {
  strLTrim( s );
  strRTrim( s );
}

void dumpToFile( const char * s, const char * pathToFile ) {
  std::ofstream os;
  os.open( pathToFile, std::ios_base::app );

  if ( ! os.is_open() ) {
    throw MyException( std::string( "Unable to open log file" ) );
  }

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
