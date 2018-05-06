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
    Wall();

    ~Wall();

    void Interact( MovingGameObject & o );
    void Interact( Pacman & o );

  private:
    // TODO
};

#endif // WALL_H
