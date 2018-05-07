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
    Ghost( const int & speed, const bool & lethal );

    /// Destructor
    ~Ghost();

  private:
    // TODO
};

#endif // GHOST_H
