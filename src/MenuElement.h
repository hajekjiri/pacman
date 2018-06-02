/// MenuElement class
/**
 * @file MenuElement.h
 */

#ifndef MENUELEMENT_H
#define MENUELEMENT_H

#include <string>
#include <functional>

class Game;

/// An element inside a menu
class MenuElement {
  public:
    /**
     * @brief Constructor
     * @param name Element's name
     * @param f Function executed when the button is pressed
     */
    MenuElement( const std::string & name,
                 const std::function<void( Game & )> & f );

    /// Destructor
    ~MenuElement();

    /// Getter for m_Action
    void DoAction( Game & g ) const;

    /// Getter for m_Name
    const std::string & GetNameConst() const;

  private:
    /// Element's name
    std::string m_Name;

    /// Element's action, executed when the element is pressed
    std::function<void( Game & )> m_Action;
};

#endif // MENUELEMENT_H
