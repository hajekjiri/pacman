/// MenuElement class
/**
 * @file MenuElement.h
 * Represents an element inside a menu with a type and a name
 */

#ifndef MENU_ELEMENT_H
#define MENU_ELEMENT_H

#include <string>
#include <functional>

class Game;

class MenuElement {
  public:
    /// Constructor
    /**
     * Create an instance of MenuElement,
     * set element's type, and name
     */
    MenuElement( const std::string & name,
                 const std::function<void( Game * )> & f );

    /// Destructor
    ~MenuElement();

    void Action( Game * g ) const;

    friend class Menu;

  private:
    /// Element's name
    std::string m_Name;

    /// Element's action
    std::function<void( Game * )> m_Action;
};

#endif // MENU_ELEMENT_H
