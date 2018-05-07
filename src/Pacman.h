/// Pacman class
/**
 * @file Pacman.h
 * Represents Pacman
 */

#ifndef PACMAN_H
#define PACMAN_H

#include "MovingGameObject.h"

class Pacman : public MovingGameObject {
  public:
    /// Constructor
    Pacman( const int & speed, const bool & lethal );

    /// Destructor
    ~Pacman();

    /// Interact with moving object
    void Interact( const MovingGameObject & o );

    /// Getter for m_Alive
    const bool IsAlive() const;

  private:
    /// Indicates whether Pacman is alive
    bool m_Alive;
};

#endif // PACMAN_H
