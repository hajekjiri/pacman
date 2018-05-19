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
    Portal( const int & id );

    /// Destructor
    ~Portal();

    void Interact( MovingGameObject & o ) override;

    void Interact( Ghost & o ) override;

    void Interact( Pacman & o ) override;

  private:
    int m_Id;
};

#endif // PORTAL_H
