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
    Pacman( const std::pair<int, int> & coords,
            const int & speed = 100,
            const bool & lethal = false );

    /// Destructor
    ~Pacman();

    /// Getter for m_Alive
    const bool IsAlive() const;

    /// Interact with Pacman
    void Interact( Ghost & o ) override;

    /// Interact with a ghost
    void Interact( Pacman & o ) override;

  private:
    /// Indicates whether Pacman is alive
    bool m_Alive;
};

#endif // PACMAN_H
