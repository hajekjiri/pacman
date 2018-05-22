/**
 * @file main.cpp
 */

/*
 * TODO:
 * - document new files for doxygen
 * - portal pair(s)
 * - pacman interaction with other objects
 * - pacman movement
 */
#include <iostream>
#include <ncurses.h>
#include <exception>
#include "Game.h"

int main() {
  initscr();
  noecho();
  Game * g = new Game();
  try {
    g->Init( "settings.cfg" );
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
