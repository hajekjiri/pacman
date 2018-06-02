/// Common functions
/**
 * @file CommonFunctions.h
  * A set of useful functions
 */

#ifndef COMMONFUNCTIONS_H
#define COMMONFUNCTIONS_H

#include <ncurses.h>
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

/**
 * @brief mvwprintw with color
 * @param w Window
 * @param y Y position
 * @param x X position
 * @param s String to write
 * @param color Color pair number
 */
void mvwprintw_color( WINDOW * w,
                      const int & y,
                      const int & x,
                      const char * const s,
                      const int & color );

#endif // COMMONFUNCTIONS_H
