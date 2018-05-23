/// GameObject class
/**
 * @file GameObject.h
 * Abstract class representing a moving object.
 */

#ifndef MOVING_GAME_OBJECT_H
#define MOVING_GAME_OBJECT_H

#include <utility>
#include "GameObject.h"

class Pacman;
class Ghost;
class Map;

class MovingGameObject : public GameObject {
  public:
    static const int DIRECTION_UP;
    static const int DIRECTION_LEFT;
    static const int DIRECTION_DOWN;
    static const int DIRECTION_RIGHT;

    /// Constructor
    MovingGameObject( const char & c,
                      const std::pair<int, int> & coords,
                      const int & speed,
                      const bool & lethal );

    /// Destructor
    virtual ~MovingGameObject() = 0;

    /// Interact with Pacman
    virtual void Interact( Ghost & o, Map & map ) = 0;

    /// Interact with a ghost
    virtual void Interact( Pacman & o, Map & map ) = 0;

    const bool IsLethal() const;

    virtual void Move( const int & direction, Map & map ) = 0;

  protected:
    /**
     * Position: first => y axis, second => x axis
     */
    std::pair<int,int> m_Coord;

    /**
     * Boolean value indicating whether
     *   the object causes Pacman to die on contact
     */
    bool m_Lethal;

    /// Movement speed
    int m_Speed;
};

#endif // MOVING_GAME_OBJECT_H
