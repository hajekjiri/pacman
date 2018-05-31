/**
 * @file main.cpp
 */

/*
 *------------------------------------------------------------------------------
 * TODO:
 * - consistency - getters: GetObject()
 *               - setters: SetObject()
 *               -    both: Object()
 * - possibly create some function(s) to save lines
 * - document new files for doxygen
 * - write documentation / tutorial
 *------------------------------------------------------------------------------
 */
#include <iostream>
#include <ncurses.h>
#include <exception>
#include "CommonFunctions.h"
#include "Game.h"
#include "GameObject.h"

int main() {
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
    return 1;
  }
  delete g;
  endwin();
  return 0;
}
