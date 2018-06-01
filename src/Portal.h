/// Portal class
/**
 * @file Portal.h
 */

#ifndef PORTAL_H
#define PORTAL_H

#include <utility>

/// Contains information about a portal
class Portal {
  public:
    /**
     * @brief Constructor
     * @param id Portal's id ( character )
     * @param coords Portal's coordinates
     */
    Portal( const char & id, const std::pair<int, int> & coords );

    /// Destructor
    ~Portal();

    /// Getter for m_Id
    const char & GetIdConst() const;

    /// Getter for m_Coords
    const std::pair<int, int> & GetCoordsConst() const;

    /// Getter/Setter for m_PairCoords
    std::pair<int, int> & GetPairCoords();

  private:
    /// Portal's id ( character )
    char m_Id;

    /// Portal's coordinates
    std::pair<int, int> m_Coords;

    /// Coordinates of portal's pair
    std::pair<int, int> m_PairCoords;
};

#endif // PORTAL_H
