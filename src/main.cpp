/**
 * @file main.cpp
 */

#include <iostream>
#include <ncurses.h>
#include <exception>
#include "Game.h"

int main() {
  initscr();
  noecho();
  Game * g = new Game();
  g->Init( "settings.cfg" );
  try {
    g->Run();
  } catch ( const std::exception & e ) {
    std::cout << "Exception thrown:\n"
              << e.what() << std::endl;
    delete g;
    endwin();
    return 1;
  }
  delete g;
  endwin();
  return 0;
}
