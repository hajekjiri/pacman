/// MovingGameObject class
/**
 * @file MovingGameObject.h
 * Class representing a moving object
 */

#ifndef MOVING_GAME_OBJECT_H
#define MOVING_GAME_OBJECT_H

#include <utility>
#include "Game.h"
#include "GameObject.h"

class MovingGameObject : public GameObject {
  public:
    /// Constructor
    MovingGameObject( const char & c,
                      const std::pair<int, int> & coords,
                      std::pair<int, int> * homeCoords,
                      const bool & lethal );

    /// Destructor
    virtual ~MovingGameObject();

    /// Getter/Setter for m_Coords
    std::pair<int, int> & GetCoords();

    /// Getter/Setter for m_Alive
    bool & GetAlive();

    /// Getter/Setter for m_Lethal
    bool & GetLethal();

    /// Getter/Setter for m_Carry
    GameObject *& GetCarry();

    const bool MovePacman( const int & direction, Game & game );

    void MoveGhost( Game & game );

  private:
    /**
     * Position: first => y axis, second => x axis
     */
    std::pair<int, int> m_Coords;

    /**
     * Home position: first => y axis, second => x axis
     * nullptr ---> no home
     */
    std::pair<int, int> * m_HomeCoords;

    /**
     * Boolean value indicating whether
     *   the object is alive
     */
    bool m_Alive;

    /**
     * Boolean value indicating whether
     * the object causes other MovingGameObject to die
     * ( only applies to Pacman )
     */
    bool m_Lethal;

    /// Object hidden behind moving object
    GameObject * m_Carry;
};

#endif // MOVING_GAME_OBJECT_H
