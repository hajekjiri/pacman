/// SolidGameObject class
/**
 * @file GameObject.h
 * Abstract class representing a solid object.
 */

#ifndef SOLID_GAME_OBJECT_H
#define SOLID_GAME_OBJECT_H

#include "GameObject.h"
#include "MovingGameObject.h"

class Pacman;
class Ghost;

class SolidGameObject : public GameObject {
  public:
    /// Constructor
    SolidGameObject( const char & c,
                     const std::pair<int, int> & coords );

    /// Destructor
    virtual ~SolidGameObject() = 0;

    /// Interact with Pacman
    virtual void Interact( Ghost & o ) = 0;

    /// Interact with a ghost
    virtual void Interact( Pacman & o ) = 0;

  protected:
};

#endif // SOLID_GAME_OBJECT_H
