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
    Portal();

    ~Portal();

    void Interact( MovingGameObject & o );
    void Interact( Pacman & o );

  private:
    int m_Score;
};

#endif // PORTAL_H
