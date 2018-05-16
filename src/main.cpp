/**
 * @file main.cpp
 */

#include <iostream>
#include <ncurses.h>
#include "Game.h"

int main() {
  //cout << "Hello World!" << endl;
/*
  initscr();
  printw( "Hello World!" );
  refresh();

  int k = getch();

  move( 1, 0 );

  printw( "Key code: %d", k );

  getch();

  endwin();
*/

  Game * g = new Game();
  g->Init( "../examples/settings.cfg" );
  g->Run();


  return 0;
}
