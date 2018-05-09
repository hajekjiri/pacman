/// SolidGameObject class
/**
 * @file GameObject.h
 * Abstract class representing a solid object.
 */

#ifndef SOLID_GAME_OBJECT_H
#define SOLID_GAME_OBJECT_H

#include "GameObject.h"
#include "MovingGameObject.h"
#include "Ghost.h"
#include "Pacman.h"

class SolidGameObject : public GameObject {
  public:
    /// Constructor
    SolidGameObject();

    /// Destructor
    virtual ~SolidGameObject() = 0;

    /// Interact with moving object
    virtual void Interact( MovingGameObject & o );

    /// Interact with Pacman
    virtual void Interact( Ghost & o ) = 0;

    /// Interact with a ghost
    virtual void Interact( Pacman & o ) = 0;

  protected:
};

#endif // SOLID_GAME_OBJECT_H
