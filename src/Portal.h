/// Portal class
/**
 * @file Portal.h
 * Represents a portal.
 */

#ifndef PORTAL_H
#define PORTAL_H

#include "SolidGameObject.h"
#include <utility>

class Map;

class Portal : public SolidGameObject {
  public:
    /// Constructor
    Portal( const int & id,
            const std::pair<int, int> & coords );

    /// Destructor
    ~Portal();

    void Interact( Ghost & o, Map & map ) override;

    void Interact( Pacman & o, Map & map ) override;

    void SetPair( Portal * p );

    int GetId() const;

  private:
    int m_Id;
    Portal * m_Pair;
};

#endif // PORTAL_H
