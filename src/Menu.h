/// Menu class
/**
 * @file Menu.h
 * Represents a menu screen
 */
#ifndef MENU_H
#define MENU_H

#include <ncurses.h>
#include <vector>
#include "MenuElement.h"

class Menu {
  public:
    static const bool DIRECTION_UP;
    static const bool DIRECTION_DOWN;

    /// Constructor
    /**
     * Create Menu, set highlighted index to -1
     * @param preset menu preset
     */
    Menu();

    /// Destructor
    ~Menu();

    /// Initialize menu
    void Init( const int & preset = 1 );

    /// Add element to menu
    void Add( const MenuElement & e );

    /// Draw menu
    void Draw();

    /// Clear menu
    void Clear();

    /// Get highlighted element's type
    const int & GetElem() const;

    friend class Game;

    friend class Game;

  private:
    /// Container for menu elements
    std::vector<MenuElement> m_Options;

    /// Menu preset:
    /**
     * 0 ---> empty menu\n
     * 1 ---> default predefined menu
     */
    int m_Preset;

    /// ncurses window
    WINDOW * m_Window;

    /// Highlighted index
    int m_HighlightedIdx;

    /// Move in the menu
    /**
     * @param direction DIRECTION_UP or DIRECTION_DOWN
     */
    void Move( const bool & direction );

    void MoveUp();

    void MoveDown();
};

#endif // MENU_H
