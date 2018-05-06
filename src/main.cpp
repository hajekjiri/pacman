/**
 * @file main.cpp
 */

#include <iostream>
#include <ncurses.h>

int main() {
  //cout << "Hello World!" << endl;
  initscr();
  printw( "Hello World!" );
  refresh();

  int k = getch();

  move( 1, 0 );

  printw( "Key code: %d", k );

  getch();

  endwin();
  return 0;
}
