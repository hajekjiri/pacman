/**
 * @file main.cpp
 */

/*
 3 - CHECK
 * TODO:
 * - Pacman must die on ghost contact, currently segfaults on contact
 1 - pacman power-up => for X turns, pacman has the ability to eat 1 ghost
 4 - powerup spawns at given location ( loaded from map ) every Xth turn, where X is loaded from config
 * - ghost movement
 2 - ghost AI - chase player; chase player if closer than X; find and destroy power-ups, otherwise random-ish
 2 - implement bfs for ghosts, update shortest path to player on every turn
 5 - ghost difficulty => every Xth turn of ghost is random, where X is loaded from config
 5 - ghost difficulty => DFS x BFS (?), sounds hard though
 * - document new files for doxygen
 -------------------------------------------------------------------------------
 * - if ghost makes a move after pacman, they can only end up stacked in this order: GameObject=>Pacman=>Ghost
 * - fix for MovingGameObject carry is already implemented, not tested though
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
    g->Init( Game::SETTINGS_FILE );
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
