/// GameObject class
/**
 * @file GameObject.h
 * Class representing an object.
 */

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <utility>

class GameObject {
  public:
    /// Constructor
    GameObject( const char & c );

    /// Destructor
    virtual ~GameObject();

    /// Getter/setter for display char
    char & Char();

    bool & Solid();

  protected:

    /// Character to display
    char m_Char;

    /**
     * Boolean value indicating if things
     *   can move through the object
     */
    bool m_Solid;
};

#endif // GAME_OBJECT_H
