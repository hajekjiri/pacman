/// Implementation of GameObject
/**
 * @file GameObject.cpp
 */

 #include "GameObject.h"

GameObject::GameObject( const char & c, const bool & lethal )
                      : m_Char( c ),
                        m_Lethal( lethal ) {
  // do nothing
}

GameObject::~GameObject() {
  // do nothing
}

char & GameObject::Char() {
  return m_Char;
}

bool & GameObject::Lethal() {
  return m_Lethal;
}
