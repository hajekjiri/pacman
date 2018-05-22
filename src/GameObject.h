/// GameObject class
/**
 * @file GameObject.h
 * Abstract class representing a game object.
 */

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <utility>

class GameObject {
  public:
    /// Constructor
    GameObject( const char & c,
                const std::pair<int, int> & coords );

    /// Destructor
    virtual ~GameObject() = 0;

    /// return display char
    const char & Char() const;

  protected:
    char m_Char;
    std::pair<int, int> m_Coords;
};

#endif // GAME_OBJECT_H
