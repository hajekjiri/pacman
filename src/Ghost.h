/// Ghost class
/**
 * @file Ghost.h
 * Represents a ghost.
 */

#ifndef GHOST_H
#define GHOST_H

#include "MovingGameObject.h"

class Ghost : public MovingGameObject {
  public:
    /// Constructor
    Ghost( const char & c,
           const std::pair<int, int> & coords,
           const int & speed = 100,
           const bool & lethal = true );

    /// Destructor
    ~Ghost();

  private:
    // TODO
};

#endif // GHOST_H
