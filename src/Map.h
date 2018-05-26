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

class Game;
class GameObject;

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
    const int & Height() const;

    /// Getter for m_Width
    const int & Width() const;

    std::vector<std::vector<GameObject*> > & Data();

    const bool ValidCoords( const std::pair<int, int> & coords ) const;

    std::vector<std::vector<GameObject*> >::iterator CheckSize();

  private:
    int m_Height;
    int m_Width;
    std::vector<std::vector<GameObject*> > m_Data;
};

#endif // MAP_H
