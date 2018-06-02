/// Common functions
/**
 * @file CommonFunctions.h
  * A set of useful functions
 */

#ifndef COMMONFUNCTIONS_H
#define COMMONFUNCTIONS_H

#include <string>
#include <utility>

/// Trim whitespace in a string from the left side
void strLTrim( std::string & s );

/// Trim whitespace in a string from the right side
void strRTrim( std::string & s );

/// Trim whitespace in a string from both sides
void strTrim( std::string & s );

/**
 * @brief Dump string to file, mainly for debugging
 * @param s String to write to file
 * @param pathToFile Path to log file
 */
void dumpToFile( const char * const s, const char * const pathToFile = "./log.txt" );

/// Is character a ghost?
const bool isGhost( const char & c );

/// Are coordinates p1 right next to p2?
const bool isNextToInLine( const std::pair<int, int> & p1,
                           const std::pair<int, int> & p2 );

#endif // COMMONFUNCTIONS_H
