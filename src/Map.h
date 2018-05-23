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

class GameObject;
class Game;

class Map {
  public:
    /// Constructor
    Map();

    /// Destructor
    ~Map();

    /// Resize map
    void Resize( const int & height, const int & width );

    /// Draw map in a window
    void Draw( WINDOW * w );

    std::vector<std::vector<GameObject*> > m_Data;

    friend class Game;

  private:
    int m_Height;
    int m_Width;

    void LoadFromFile( const std::string & path, Game & game );
};

#endif // MAP_H
