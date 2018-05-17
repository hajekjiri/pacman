/**
 * @file main.cpp
 */

#include <iostream>
#include <ncurses.h>
#include "Game.h"

int main() {
  initscr();
  noecho();
  Game * g = new Game();
  g->Init( "../examples/settings.cfg" );
  try {
    g->Run();
  } catch ( ... ) {
    delete g;
    endwin();
    return 1;
  }
  delete g;
  endwin();
  return 0;
}
