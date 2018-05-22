/// Wall class
/**
 * @file Wall.h
 * Represents a wall.
 */

#ifndef WALL_H
#define WALL_H

#include "SolidGameObject.h"

class Map;

class Wall : public SolidGameObject {
  public:
    /// Constructor
    Wall( const std::pair<int, int> & coords );

    /// Destructor
    ~Wall();

    void Interact( Ghost & o, Map & map ) override;

    void Interact( Pacman & o, Map & map ) override;

  private:
    // TODO
};

#endif // WALL_H
