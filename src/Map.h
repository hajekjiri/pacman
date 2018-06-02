/// Map class
/**
 * @file Map.h
 */

#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>
#include <ncurses.h>
#include "GameObject.h"

class Game;

/// Game map
class Map {
  public:
    static const int COLOR_PACMAN;
    static const int COLOR_GHOST_A;
    static const int COLOR_GHOST_B;
    static const int COLOR_GHOST_C;
    static const int COLOR_WALL;
    static const int COLOR_BLANK;
    static const int COLOR_COIN;
    static const int COLOR_BONUS_COIN;
    static const int COLOR_PORTAL;

    /// Constructor
    Map();

    /// Destructor
    ~Map();

    /**
     * @brief Draw map into a window
     * @param w Window to draw the map into
     */
    void Draw( WINDOW * w );

    /**
     * @brief Load map from file
     * @param path Path to map file
     * @param game Game that is going to use the map
     */
    void LoadFromFile( const std::string & path, Game & game );

    /// Getter for m_Height
    const int & GetHeightConst() const;

    /// Getter for m_Width
    const int & GetWidthConst() const;

    /// Getter/Setter for m_Data
    std::vector<std::vector<GameObject*> > & GetData();

    /**
     * @brief Check if given coordinates are valid
     * @param coords Coordinates to check
     * @return Valid coordinates => true, otherwise false
     */
    const bool ValidCoords( const std::pair<int, int> & coords ) const;

    /**
     * @brief Check map size
     * @return Iterator to last row
     */
    std::vector<std::vector<GameObject*> >::iterator CheckSize();

  private:
    /// Height of the map
    int m_Height;

    /// Width of the map
    int m_Width;

    /// Container for the map data
    std::vector<std::vector<GameObject*> > m_Data;
};

#endif // MAP_H
