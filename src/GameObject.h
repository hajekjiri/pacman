/// GameObject class
/**
 * @file GameObject.h
 */

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

/// Basic game object
class GameObject {
  public:
    /**
     * @briefConstructor
     * @param c Object's character
     */
    GameObject( const char & c );

    /// Destructor
    virtual ~GameObject();

    /// Getter/setter for m_CHar
    char & GetChar();

  protected:

    /// Character to display
    char m_Char;
};

#endif // GAMEOBJECT_H
