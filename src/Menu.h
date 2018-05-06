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
    /// Constructor
    /**
     * Create Menu, set highlighted index to -1
     * @param preset menu preset
     */
    Menu( const int & preset = 1 );

    /// Destructor
    ~Menu();

    /// Add element to menu
    void Add( const MenuElement & e );

    /// Draw menu
    void Draw();

    /// Clear menu
    void Clear();

    /// Get highlighted element's type
    const int & GetElem() const;

    void MoveUp();

    void MoveDown();

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
     * @param direction true => down, false => up
     */
    void Move( const bool & direction );
};

#endif // MENU_H
