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

    std::vector<std::vector<GameObject*> > & Data();

    const bool ValidCoords( const std::pair<int, int> & coords ) const;

    std::vector<std::vector<GameObject*> >::iterator CheckSize();

    friend class Game;

  private:
    int m_Height;
    int m_Width;
    std::vector<std::vector<GameObject*> > m_Data;

    void LoadFromFile( const std::string & path, Game & game );
};

#endif // MAP_H
