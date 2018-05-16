/// Game class
/**
 * @file Game.h
 * Represents the Pacman game itself as an object
 */

#ifndef GAME_H
#define GAME_H

#include <ncurses.h>
#include <string>
#include "Menu.h"
#include "Map.h"
#include "Pacman.h"

class Ghost;

class Game {
  public:
    // game state declarations
    static const int STATE_RUNNING;
    static const int STATE_PAUSED;
    static const int STATE_MENU;
    static const int STATE_HELP;

    /// Constructor
    Game();

    /// Destructor
    ~Game();

    /**
     * Load settings from file,
     * use empty string(s) ( "" ) for default settings. (?)
     */
    void Init( const std::string & pathToConfig );

    /// Run game
    void Run();

    /// Start playing the game
    void Play();

    /// Change state of game
    void ChangeState( const int & state );

  private:
    int m_GameState;
    Map m_Map;
    WINDOW * m_Window;
    Menu m_Menu;
    std::vector<Ghost> m_Ghosts;
    Pacman m_Pacman;
};

#endif // GAME_H
