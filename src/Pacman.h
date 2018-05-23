/// Pacman class
/**
 * @file Pacman.h
 * Represents Pacman
 */

#ifndef PACMAN_H
#define PACMAN_H

#include "MovingGameObject.h"

class Map;

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
    void Interact( Ghost & o, Map & map ) override;

    /// Interact with a ghost
    void Interact( Pacman & o, Map & map ) override;

    void Move( const int & direction, Map & map ) override;

  private:
    //
};

#endif // PACMAN_H
