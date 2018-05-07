/// Wall class
/**
 * @file Wall.h
 * Represents a wall.
 */

#ifndef WALL_H
#define WALL_H

#include "SolidGameObject.h"

class Wall : public SolidGameObject {
  public:
    /// Constructor
    Wall();

    /// Destructor
    ~Wall();

    void Interact( MovingGameObject & o ) override;

    void Interact( Ghost & o ) override;

    void Interact( Pacman & o ) override;

  private:
    // TODO
};

#endif // WALL_H
