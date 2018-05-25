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
 * - fix Game::Reset()
 * - test multiple ghosts
 * - make path finder return distance, if no path exists => return -1
 5 - ghost difficulty => allow ghosts to use portals ( via cfg ), add as parameter to path finder
 * - configure path finder to optionally allow use of portals
 2 - ghost AI:
 *     - A => chase player
 *     - B => chase player if player is closer than X
 *     - C => if bonus exists, chase bonus, if sitting on top of bonus and player gets close => chase player
 * - document new files for doxygen
 *--------------------------------------------------------------------------------------------------
 * - if ghost makes a move after pacman, they can only end up stacked in this order: GameObject=>Pacman=>Ghost
 * - fix for MovingGameObject carry is already implemented, not tested though
 *
 *     - do NOT clear carries to prevent memory leaks, carries are not deleted in Map's destruction
 *
 * // probs not gonna do ghost stacking because of possibly tons of carry searching
 * - pacman steps on stacked ghosts => a) dies to first ghost
 *                                     b) eats first ghost, if carry is a ghost, dies to second ghost
 * - ghost movement => check if ghost is a carry of some other ghost,
 *     if so, then give the "parent" ghost child ghosts' carry and then make the child ghosts' move,
 *     setting child ghost's carry to nullptr and not placing it behind to prevent map corruption
 */
#include <iostream>
#include <ncurses.h>
#include <exception>
#include <sstream>
#include "CommonFunctions.h"
#include "Game.h"
#include "GameObject.h"

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
    dumpToFile( "Exception thrown:\n" );
    std::ostringstream oss;
    oss << e.what() << std::endl;
    dumpToFile( oss.str().data() );
    dumpToFile( "=========================================================\n" );
    return 1;
  }
  delete g;
  endwin();
  dumpToFile( "Program finished\n" );
  dumpToFile( "=========================================================\n" );
  dumpToFile( "=========================================================\n" );
  return 0;
}
