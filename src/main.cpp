/**
 * @file main.cpp
 */

/*
 * TODO:
 * - document new files for doxygen
 * - ghost movement
 * - ghost AI
 */
#include <iostream>
#include <ncurses.h>
#include <exception>
#include "Game.h"

int main() {
  initscr();
  noecho();
  curs_set( 0 );
  nodelay( stdscr, false );
  Game * g = new Game();
  try {
    g->Init( "settings.cfg" );
    g->Run();
  } catch ( const std::exception & e ) {
    endwin();
    std::cout << "Exception thrown:\n"
              << e.what() << std::endl;
    delete g;
    return 1;
  }
  endwin();
  delete g;
  return 0;
}
