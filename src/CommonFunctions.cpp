#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <algorithm>
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

void dumpToFile( const char * s, const char * pathToFile = "./log.txt" ) {
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
