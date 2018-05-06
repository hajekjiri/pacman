/// GameObject class
/**
 * @file GameObject.h
 * Abstract class representing a moving object.
 */

#ifndef MOVING_GAME_OBJECT_H
#define MOVING_GAME_OBJECT_H

#include <utility>
#include "GameObject.h"

class MovingGameObject : public GameObject {
  public:
    static const int DIRECTION_LEFT;
    static const int DIRECTION_RIGHT;
    static const int DIRECTION_UP;
    static const int DIRECTION_DOWN;

    /// Constructor
    MovingGameObject();

    /// Destructor
    virtual ~MovingGameObject() = 0;

    const bool IsLethal() const;

    void Move( const int & direction );

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
