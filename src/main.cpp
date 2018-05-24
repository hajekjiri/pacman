/**
 * @file main.cpp
 */

/*
 * DONE:
 * 1 - Basic rules
 * 3 - Load map from file
 * 4 - Variable game diff ( load from cfg )
 * TODO:
 * 2 - ghost AI
 * 5 - ghost diff ( load from cfg )
 *--------------------------------------------------------------------------------------------------
 * - ghost movement, ghosts cannot walk through each other
 * - ghost interaction: ghost => pacman
 2 - ghost AI - chase player; chase player if closer than X; find and destroy power-ups, otherwise random-ish
 2 - implement bfs for ghosts, update shortest path to player on every turn
 5 - ghost difficulty => every Xth turn of ghost is random, where X is loaded from config
 5 - ghost difficulty => DFS x BFS (?), sounds hard though
 * - document new files for doxygen
 *--------------------------------------------------------------------------------------------------
 * - if ghost makes a move after pacman, they can only end up stacked in this order: GameObject=>Pacman=>Ghost
 * - fix for MovingGameObject carry is already implemented, not tested though
 *
 * - pacman steps on stacked ghosts => a) dies to first ghost
 *                                     b) eats first ghost, if carry is a ghost, dies to second ghost
 *     - do NOT clear carries to prevent memory leaks, carries are not deleted in Map's destruction
 *
 * // probs not gonna do ghost stacking because of possibly tons of carry searching
 * - ghost movement => check if ghost is a carry of some other ghost,
 *     if so, then give the "parent" ghost child ghosts' carry and then make the child ghosts' move,
 *     setting child ghost's carry to nullptr and not placing it behind to prevent map corruption
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
