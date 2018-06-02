/// MovingGameObject class
/**
 * @file MovingGameObject.h
 */

#ifndef MOVINGGAMEOBJECT_H
#define MOVINGGAMEOBJECT_H

#include <utility>
#include "Game.h"
#include "GameObject.h"

/// A moving object
class MovingGameObject : public GameObject {
  public:
    /**
     * @brief Constructor
     * @param c Object's character
     * @param coords Object's coordinates
     * @param homeCoords Object's home coordinates
     * @param lethal Whether the object is lethal
     */
    MovingGameObject( const char & c,
                      const std::pair<int, int> & coords,
                      std::pair<int, int> * const homeCoords,
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

    /// Move function for Pacman
    const bool MovePacman( const int & direction, Game & game );

    /// Move function for Ghosts
    void MoveGhost( Game & game );

  private:
    /// Object's position
    std::pair<int, int> m_Coords;

    /// Object's home position, nullptr => no home
    std::pair<int, int> * m_HomeCoords;

    /// Whether the objects is alive
    bool m_Alive;

    /// Whether the object causes its enemy to die, if both pacman and a ghost are lethal, pacman eats the ghost
    bool m_Lethal;

    /// Object hidden behind the moving object
    GameObject * m_Carry;
};

#endif // MOVINGGAMEOBJECT_H
