/// Main program
/**
 * @file main.cpp
 */

/*
 *------------------------------------------------------------------------------
 * TODO:
 * - write documentation / tutorial
 * - possibly create some function(s) to save lines <--- meh
 * - last checks for memory leaks and errors before submitting
 * - remove this todo list
 *------------------------------------------------------------------------------
 */


#include <iostream>
#include <ncurses.h>
#include <exception>
#include "CommonFunctions.h"
#include "Game.h"
#include "GameObject.h"

int main() {
  // init ncurses
  initscr();
  start_color();
  noecho();
  curs_set( 0 );
  nodelay( stdscr, false );

  // create and play game
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
