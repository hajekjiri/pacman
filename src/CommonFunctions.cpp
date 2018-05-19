#ifndef COMMON_FUNCTIONS_H
#define COMMON_FUNCTIONS_H

#include <string>
#include <cctype>
#include <algorithm>

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

#endif // COMMON_FUNCTIONS_H
