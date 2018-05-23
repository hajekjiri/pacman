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
    GameObject( const char & c, const bool & lethal );

    /// Destructor
    virtual ~GameObject();

    /// Getter/setter for display char
    char & Char();

    /// Getter/Setter for m_Lethal
    bool & Lethal();

  protected:

    /// Character to display
    char m_Char;

    /**
     * Boolean value indicating whether
     *   the object causes Pacman/Ghost to die on contact with Ghost/Pacman
     */
    bool m_Lethal;
};

#endif // GAME_OBJECT_H
