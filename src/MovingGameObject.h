/// GameObject class
/**
 * @file GameObject.h
 * Class representing a moving object.
 */

#ifndef MOVING_GAME_OBJECT_H
#define MOVING_GAME_OBJECT_H

#include <utility>
#include "GameObject.h"

class MovingGameObject : public GameObject {
  public:
    /// Constructor
    MovingGameObject( const char & c,
                      const std::pair<int, int> & coords,
                      const int & speed,
                      const bool & lethal );

    /// Destructor
    virtual ~MovingGameObject();

    /// Getter/Setter for m_Lethal
    std::pair<int, int> & Coords();

    /// Getter/Setter for m_Lethal
    bool & Lethal();

    /// Getter/Setter for m_Alive
    bool & Alive();

    /// Getter/Setter for m_Speed
    int & Speed();

    void Move( const int & direction );

  private:
    /**
     * Position: first => y axis, second => x axis
     */
    std::pair<int,int> m_Coords;

    /**
     * Boolean value indicating whether
     *   the object causes Pacman to die on contact
     */
    bool m_Lethal;

    /**
     * Boolean value indicating whether
     *   the object is alive
     */
    bool m_Alive;

    /// Movement speed ( moves per 1 game turn )
    int m_Speed;
};

#endif // MOVING_GAME_OBJECT_H
