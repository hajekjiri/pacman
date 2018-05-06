/// Game class
/**
 * @file Game.h
 * Represents the Pacman game itself as an object
 */

#ifndef GAME_H
#define GAME_H

#include <ncurses.h>
#include "Menu.h"
#include "Map.h"
#include "Pacman.h"
#include "Ghost.h"

class Game {
  public:
    // game state declarations
    static const int STATE_RUNNING;
    static const int STATE_PAUSED;
    static const int STATE_MENU;

    Game();

    ~Game();

    /**
     * Load map and difficulty settings from file(s),
     * use empty string(s) ( "" ) for default settings/map.
     */
    void Init( const std::string & pathToConfig,
               const std::string & pathToMap );

    // Run game
    void Run();

  private:
    int m_GameState;
    Map m_Map;
    WINDOW * m_Window;
    Menu m_Menu;
    std::vector<Ghost> m_Ghosts;
    Pacman m_Pacman;

    // Change state of game
    void ChangeState( const int & state );
};

#endif // GAME_H
