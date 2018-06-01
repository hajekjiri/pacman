/// Menu class
/**
 * @file Menu.h
 */
#ifndef MENU_H
#define MENU_H

#include <ncurses.h>
#include <vector>
#include "MenuElement.h"

/// Menu screen
class Menu {
  public:
    static const bool DIRECTION_UP;
    static const bool DIRECTION_DOWN;

    /// Constructor
    Menu();

    /// Destructor
    ~Menu();

    /// Initialize menu
    void Init();

    /**
     * @brief Add element to menu
     * @param e Element to add
     */
    void Add( const MenuElement & e );

    /// Draw menu
    void Draw();

    /// Clear menu
    void Clear();

    /// Move up in the menu
    void MoveUp();

    /// Move down in the menu
    void MoveDown();

    /// Getter/Setter for m_Window
    WINDOW *& GetWindow();

    /// Getter for m_Options
    const std::vector<MenuElement> & GetOptionsConst() const;

    /// Getter for m_HighlightedIdx
    const int & GetHighlightedIdxConst();

  private:
    /// Container for menu elements
    std::vector<MenuElement> m_Options;

    /// Ncurses window
    WINDOW * m_Window;

    /// Index of highlighted element
    int m_HighlightedIdx;

    /**
     * @brief Move in the menu
     * @param direction DIRECTION_UP or DIRECTION_DOWN
     */
    void Move( const bool & direction );
};

#endif // MENU_H
