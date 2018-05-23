#ifndef PORTAL_H
#define PORTAL_H

#include <utility>

class Portal {
  public:
    Portal( const char & id, const std::pair<int, int> & coords );

    ~Portal();

    char & Id();

    std::pair<int, int> & Coords();

    std::pair<int, int> & PairCoords();

  private:
    char m_Id;
    std::pair<int, int> m_Coords;
    std::pair<int, int> m_PairCoords;
};

#endif // PORTAL_H
