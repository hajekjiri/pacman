/// Portal class
/**
 * @file Portal.h
 * Represents a portal.
 */

#ifndef PORTAL_H
#define PORTAL_H

#include "SolidGameObject.h"

class Portal : public SolidGameObject {
  public:
    /// Constructor
    Portal();

    /// Destructor
    ~Portal();

    void Interact( MovingGameObject & o ) override;

    void Interact( Ghost & o ) override;

    void Interact( Pacman & o ) override;

  private:
    // TODO
};

#endif // PORTAL_H
