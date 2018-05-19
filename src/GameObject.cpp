/// Implementation of GameObject
/**
 * @file GameObject.cpp
 */

 #include "GameObject.h"

GameObject::GameObject( const char & c )
                      : m_Char( c ) {
  // TODO
}

GameObject::~GameObject() {
  // TODO
}

const char & GameObject::Char() const {
  return m_Char;
}
