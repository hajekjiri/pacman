/// Game class
/**
 * @file Game.h
 */

#ifndef GAME_H
#define GAME_H

#include <map>
#include <ncurses.h>
#include <string>
#include "Map.h"
#include "Menu.h"
#include "MovingGameObject.h"
#include "Portal.h"
#include "Setting.h"

/// Game as an object
class Game {
  public:
    // game state declarations
    static const int STATE_RUNNING;
    static const int STATE_PAUSED;
    static const int STATE_MENU;
    static const int STATE_HELP;
    static const int STATE_END;
    static const int STATE_EXIT;

    static const int MODE_CLASSIC;
    static const int MODE_SURVIVAL;

    static const int RESULT_WIN;
    static const int RESULT_LOSS;

    /**
     * @brief Constructor
     * @param pathToCfg path to settings file
     */
    Game( const char * const pathToCfg );

    /// Destructor
    ~Game();

    /**
     * @brief Initialize game
     */
    void Init();

    /// Run game
    void Run();

    /// Start playing the game
    void Play();

    /// Check if there are still some coins left
    const bool CoinsLeft();

    /// Getter/Setter for m_Map
    Map & GetMap();

    /// Getter/Setter for m_Menu
    Menu & GetMenu();

    /// Getter/Setter for m_Portals
    std::vector<Portal*> & GetPortals();

    /// Getter/Setter for m_Score
    int & GetScore();

    /// Getter/Setter for m_Mode
    int & GetMode();

    /// Getter/Setter for m_BonusTurns
    int & GetBonusTurns();

    /// Getter/Setter for m_RespawnBonusTurnNo
    int & GetRespawnBonusTurnNo();

    /// Getter for m_Turn
    const int & GetTurnsConst() const;

    /// Getter/Setter for m_Settings
    std::map<std::string, Setting> & GetSettings();

    /// Getter/Setter for m_Pacman
    MovingGameObject *& GetPacman();

    /// Getter/Setter for m_Ghosts
    std::vector<MovingGameObject*> & GetGhosts();

    /// Getter/Setter for m_BonusCoords
    std::vector<std::pair<int, int> > & GetBonusCoords();

    /**
     * @brief Search in settings
     * @param key Setting to search for
     * @return Class setting with value, throw exception if not found
     */
    const Setting & FindSetting( const std::string & key ) const;

    /**
     * @brief Load settings from cfg file
     */
    void LoadCfg();

    /**
     * @brief Change state of the game
     * @param state New game state
     */
    void ChangeState( const int & state );

    /// Respawn bonuses
    void RespawnBonus();

    /// Draw game information ( score, turns, etc. )
    void DrawInfo();

  private:
    /// Path to settings config file
    std::string m_SettingsFile;

    /// State of the game
    int m_GameState;

    /// Game's map
    Map m_Map;

    /// Main play window
    WINDOW * m_Window;

    /// Pause window
    WINDOW * m_PauseWin;

    /// Information window
    WINDOW * m_InfoWin;

    /// Game's menu
    Menu m_Menu;

    /// Container with portals
    std::vector<Portal*> m_Portals;

    /// Container with ghosts
    std::vector<MovingGameObject*> m_Ghosts;

    /// Pointer to Pacman
    MovingGameObject * m_Pacman;

    /// Container with settings
    std::map<std::string, Setting> m_Settings;

    /// Container with coordinates of bonuses
    std::vector<std::pair<int, int> > m_BonusCoords;

    /// Amount of coins collected by player
    int m_Score;

    /// Amount of turns made by player
    int m_Turns;

    /// Game mode => classic or survival
    int m_Mode;

    /// Amount of turns Pacman is able to eat ghosts
    int m_BonusTurns;

    /// Turn number in which bonus coin(s) are supposed to be respawned
    int m_RespawnBonusTurnNo;

    /// Result of the game, win => 1, loss => -1
    int m_Result;

    /// Reset game
    void Reset();
};

#endif // GAME_H
