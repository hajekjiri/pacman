/// Game class
/**
 * @file Game.h
 * Represents the Pacman game itself as an object
 */

#ifndef GAME_H
#define GAME_H

#include <ncurses.h>
#include <string>
#include <map>
#include "Menu.h"
#include "Map.h"
#include "Portal.h"

class MovingGameObject;

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
    void Init( const std::string & pathToCfg );

    /// Run game
    void Run();

    /// Start playing the game
    void Play();

    /// Make a game turn
    void Turn( const int & k );

    /// Reset game
    void Reset();

    /// Getter/Setter for m_Map
    Map & GetMap();

    /// Getter/Setter for m_Portals
    std::vector<Portal*> & Portals();

    /// Getter/Setter for m_Score
    int & Score();

    /// Change state of game
    void ChangeState( const int & state );

    friend class Map;

  private:
    int m_GameState;
    Map m_Map;
    WINDOW * m_Window;
    WINDOW * m_PauseWin;
    Menu m_Menu;
    std::vector<Portal*> m_Portals;
    std::vector<MovingGameObject*> m_Ghosts;
    MovingGameObject * m_Pacman;
    std::map<std::string, std::string> m_Settings;
    /**
     * Score, or amount of coins collected by player
     */
    int m_Score;
    int m_Turns;

    void LoadCfg( const std::string & pathToCfg );
};

#endif // GAME_H
