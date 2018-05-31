/// GameObject class
/**
 * @file GameObject.h
 * Class representing an object
 */

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

class GameObject {
  public:
    /// Constructor
    GameObject( const char & c );

    /// Destructor
    virtual ~GameObject();

    /// Getter/setter for display char
    char & Char();

  protected:

    /// Character to display
    char m_Char;
};

#endif // GAME_OBJECT_H
