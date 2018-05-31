/// Portal class
/**
 * @file Portal.h
 * Contains information about a portal
 */

#ifndef PORTAL_H
#define PORTAL_H

#include <utility>

class Portal {
  public:
    /// Constructor
    Portal( const char & id, const std::pair<int, int> & coords );

    /// Destructor
    ~Portal();

    ///
    const char & GetIdConst() const;

    const std::pair<int, int> & GetCoordsConst() const;

    std::pair<int, int> & GetPairCoords();

  private:
    char m_Id;
    std::pair<int, int> m_Coords;
    std::pair<int, int> m_PairCoords;
};

#endif // PORTAL_H
