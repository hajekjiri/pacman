/// Implementation of GameObject
/**
 * @file GameObject.cpp
 */

 #include "GameObject.h"

GameObject::GameObject( const char & c )
                      : m_Char( c ) {
  // do nothing
}

GameObject::~GameObject() {
  // do nothing
}

char & GameObject::GetChar() {
  return m_Char;
}
