#ifndef COMMON_FUNCTIONS_H
#define COMMON_FUNCTIONS_H

void strLTrim( std::string & s );

void strRTrim( std::string & s );

void strTrim( std::string & s );

void dumpToFile( const char * s, const char * pathToFile = "./log.txt" );

const bool isGhost( const char & c );

const bool isNextToInLine( const std::pair<int, int> & p1, const std::pair<int, int> & p2 );

#endif // COMMON_FUNCTIONS_H
