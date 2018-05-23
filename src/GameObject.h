/// GameObject class
/**
 * @file GameObject.h
 * Abstract class representing a game object.
 */

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <utility>

class Pacman;
class Ghost;
class Map;

class GameObject {
  public:
    /// Constructor
    GameObject( const char & c,
                const std::pair<int, int> & coords );

    /// Destructor
    virtual ~GameObject() = 0;

    /// return display char
    const char & Char() const;

    /// Interact with Pacman
    virtual void Interact( Ghost * o, Map & map ) = 0;

    /// Interact with a ghost
    virtual void Interact( Pacman * o, Map & map ) = 0;

    std::pair<int, int> m_Coords;

  protected:
    char m_Char;
};

#endif // GAME_OBJECT_H
