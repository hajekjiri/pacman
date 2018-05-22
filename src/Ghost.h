/// Ghost class
/**
 * @file Ghost.h
 * Represents a ghost.
 */

#ifndef GHOST_H
#define GHOST_H

#include "MovingGameObject.h"

class Map;

class Ghost : public MovingGameObject {
  public:
    /// Constructor
    Ghost( const char & c,
           const std::pair<int, int> & coords,
           const int & speed = 100,
           const bool & lethal = true );

    /// Destructor
    ~Ghost();

    /// Interact with Pacman
    void Interact( Ghost & o, Map & map ) override;

    /// Interact with a ghost
    void Interact( Pacman & o, Map & map ) override;

    void Move( const int & direction ) override;

  private:
    // TODO
};

#endif // GHOST_H
