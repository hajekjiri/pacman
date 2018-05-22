/// Portal class
/**
 * @file Portal.h
 * Represents a portal.
 */

#ifndef PORTAL_H
#define PORTAL_H

#include "SolidGameObject.h"
#include <utility>

class Portal : public SolidGameObject {
  public:
    /// Constructor
    Portal( const int & id,
            std::pair<int, int> coords );

    /// Destructor
    ~Portal();

    void Interact( MovingGameObject & o ) override;

    void Interact( Ghost & o ) override;

    void Interact( Pacman & o ) override;

    void SetPair( Portal * p );

    int GetId() const;

  private:
    int m_Id;
    std::pair<int, int> m_Coords;
    Portal * m_Pair;
};

#endif // PORTAL_H
