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
    static const int STATE_END;
    static const int STATE_EXIT;

    static const char * SETTINGS_FILE;

    static const int MODE_CLASSIC;
    static const int MODE_SURVIVAL;

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

    /// Reset game
    void Reset();

    const bool CoinsLeft();

    /// Getter/Setter for m_Map
    Map & GetMap();

    /// Getter/Setter for m_Menu
    Menu & GetMenu();

    /// Getter/Setter for m_Portals
    std::vector<Portal*> & Portals();

    /// Getter/Setter for m_Score
    int & Score();

    /// Getter/Setter for m_Mode
    int & Mode();

    /// Getter/Setter for m_BonusTurns
    int & BonusTurns();

    /// Getter/Setter for m_Settings
    std::map<std::string, std::string> & Settings();

    /// Getter/Setter for m_Pacman
    MovingGameObject * Pacman();

    /// Getter/Setter for m_Ghosts
    std::vector<MovingGameObject*> & Ghosts();

    /// Getter/Setter for m_BonusCoords
    std::vector<std::pair<int, int> > & BonusCoords();

    /// Function for quicker settings search
    const char * Setting( const std::string & key ) const;

    /// Load settings from cfg file
    void LoadCfg( const std::string & pathToCfg );

    /// Change state of game
    void ChangeState( const int & state );

    /// Respawn bonuses
    void RespawnBonus();

    /// Draw info ( score, turns, etc. )
    void DrawInfo();

    friend class Map;

  private:
    int m_GameState;
    Map m_Map;
    WINDOW * m_Window;
    WINDOW * m_PauseWin;
    WINDOW * m_InfoWin;
    Menu m_Menu;
    std::vector<Portal*> m_Portals;
    std::vector<MovingGameObject*> m_Ghosts;
    MovingGameObject * m_Pacman;
    std::map<std::string, std::string> m_Settings;
    std::vector<std::pair<int, int> > m_BonusCoords;

    /**
     * Score => amount of coins collected by player
     */
    int m_Score;

    /**
     * Turns => amount of turns made by player
     */
    int m_Turns;

    /**
     * Game mode
     */
    int m_Mode;

    /**
     * Amount of turns where Pacman is able to eat ghosts
     */
    int m_BonusTurns;
};

#endif // GAME_H
