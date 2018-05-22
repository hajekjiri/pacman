/// Implementation of GameObject
/**
 * @file GameObject.cpp
 */

 #include "GameObject.h"

GameObject::GameObject( const char & c,
                        const std::pair<int, int> & coords )
                      : m_Char( c ),
                        m_Coords( coords ) {
  // TODO
}

GameObject::~GameObject() {
  // TODO
}

const char & GameObject::Char() const {
  return m_Char;
}
