/// Map class
/**
 * @file Map.h
 * Represents a game map
 */

#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>
#include <ncurses.h>
#include "GameObject.h"

class Game;

class Map {
  public:
    /// Constructor
    Map();

    /// Destructor
    ~Map();

    /// Draw map into a window
    void Draw( WINDOW * w );

    /// Load map from file
    void LoadFromFile( const std::string & path, Game & game );

    /// Getter for m_Height
    const int & GetHeightConst() const;

    /// Getter for m_Width
    const int & GetWidthConst() const;

    std::vector<std::vector<GameObject*> > & GetData();

    const bool ValidCoords( const std::pair<int, int> & coords ) const;

    /**
     * Check map size, return iterator to last row
     */
    std::vector<std::vector<GameObject*> >::iterator CheckSize();

  private:
    int m_Height;
    int m_Width;
    std::vector<std::vector<GameObject*> > m_Data;
};

#endif // MAP_H
