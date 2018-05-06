/// MenuElement class
/**
 * @file MenuElement.h
 * Represents an element inside a menu with a type and a name
 */

#ifndef MENU_ELEMENT_H
#define MENU_ELEMENT_H

#include <string>
#include <functional>

class MenuElement {
  public:
    static const int ELEMENT_INVALID;
    static const int ELEMENT_PLAY;
    static const int ELEMENT_HELP;
    static const int ELEMENT_EXIT;

    /// Constructor
    /**
     * Create an instance of MenuElement,
     * set element's type, and name
     */
    MenuElement( const int & type );

    /// Destructor
    ~MenuElement();

    /// Get element's type
    const int & GetType() const;

  private:
    /// Element's type
    int m_Type;

    /// Element's name
    std::string m_Name;
};

#endif // MENU_ELEMENT_H
