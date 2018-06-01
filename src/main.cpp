/// Main program
/**
 * @file main.cpp
 */

/*
--------------------------------------------------------------------------------
TODO:
- Look for:
  - non-const parameters for no reason
  - repeating code
- comment code
--------------------------------------------------------------------------------
*/

#include <iostream>
#include <sstream>
#include <ncurses.h>
#include <exception>
#include "CommonFunctions.h"
#include "Game.h"
#include "GameObject.h"
#include "MyException.h"

/**
 * @brief The main program
 * @param argc Amount of arguments
 * @param argv array of arguments
 * @return 0 => success, 1 => error
 */
int main( int argc, const char * const * const argv ) {
  // const pointer to const pointer to const value^

  if ( argc != 2 ) {
    std::ostringstream oss;
    if ( argc > 2 ) {
      oss << "Too many arguments\n";
    } else {
      oss << "Too few arguments\n";
    }
    oss << "Got " << argc - 1 << ", expected 1";
    throw MyException( oss.str().data() );
  }

  // init ncurses
  initscr();
  start_color();
  noecho();
  curs_set( 0 );
  nodelay( stdscr, false );

  // create and play game
  Game * g = new Game( argv[ 1 ] );
  try {
    g->Init();
    g->Run();
  } catch ( const std::exception & e ) {
    delete g;
    endwin();
    std::cout << "Exception thrown:\n"
              << e.what() << std::endl;
    return 1;
  }
  delete g;
  endwin();
  return 0;
}
