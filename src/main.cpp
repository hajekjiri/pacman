/**
 * @file main.cpp
 */

/*
 * - create functions to save lines
 *
 * - write documentation
 * - document new files for doxygen
 *--------------------------------------------------------------------------------------------------
 */
#include <iostream>
#include <ncurses.h>
#include <exception>
#include <sstream>
#include "CommonFunctions.h"
#include "Game.h"
#include "GameObject.h"

int main() {
  dumpToFile( "=========================================================\n" );
  initscr();
  start_color();
  noecho();
  curs_set( 0 );
  nodelay( stdscr, false );
  Game * g = new Game();
  try {
    g->Init( Game::SETTINGS_FILE );
    g->Run();
  } catch ( const std::exception & e ) {
    delete g;
    endwin();
    std::cout << "Exception thrown:\n"
              << e.what() << std::endl;
    dumpToFile( "Exception thrown:\n" );
    std::ostringstream oss;
    oss << e.what() << std::endl;
    dumpToFile( oss.str().data() );
    dumpToFile( "=========================================================\n" );
    return 1;
  }
  delete g;
  endwin();
  dumpToFile( "Program finished\n" );
  dumpToFile( "=========================================================\n" );
  return 0;
}
