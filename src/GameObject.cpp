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

char & GameObject::Char() {
  return m_Char;
}

bool & GameObject::Solid() {
  return m_Solid;
}
