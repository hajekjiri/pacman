/// SolidGameObject class
/**
 * @file GameObject.h
 * Abstract class representing a solid object.
 */

#ifndef SOLID_GAME_OBJECT_H
#define SOLID_GAME_OBJECT_H

#include "GameObject.h"
#include "MovingGameObject.h"
#include "Pacman.h"

class SolidGameObject : public GameObject {
  public:
    SolidGameObject();

    ~SolidGameObject() = 0;

    virtual void Interact( MovingGameObject & o ) = 0;
    virtual void Interact( Pacman & o ) = 0;

  protected:
};

#endif // SOLID_GAME_OBJECT_H
