/// GameObject class
/**
 * @file GameObject.h
 * Abstract class representing a game object.
 */

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

class GameObject {
  public:
    /// Constructor
    GameObject( const char & c );

    /// Destructor
    virtual ~GameObject() = 0;

    /// return display char
    const char & Char() const;

  protected:
    char m_Char;
};

#endif // GAME_OBJECT_H
