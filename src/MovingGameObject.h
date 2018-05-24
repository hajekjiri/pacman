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

    /// Getter/Setter for m_Alive
    bool & Alive();

    /// Getter/Setter for m_Speed
    int & Speed();

    /// Getter/Setter for m_Lethal
    bool & Lethal();

    /// Getter/Setter for m_Carry
    GameObject * Carry();

    const bool MovePacman( const int & direction, Game & game );

    void MoveGhost( Game & game );

  private:
    /**
     * Position: first => y axis, second => x axis
     */
    std::pair<int, int> m_Coords;

    /**
     * Boolean value indicating whether
     *   the object is alive
     */
    bool m_Alive;

    /// Movement speed ( # of moves per 1 game turn )
    int m_Speed;

    /**
     * Boolean value indicating whether
     *   the object causes Pacman/Ghost to die on contact with Ghost/Pacman
     */
    bool m_Lethal;

    /// Object hidden behind moving object
    GameObject * m_Carry;
};

#endif // MOVING_GAME_OBJECT_H
