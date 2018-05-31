/// Implementation of MenuElement
/**
 * @file MenuElement.cpp
 */


#include "MenuElement.h"

MenuElement::MenuElement( const std::string & name,
                          const std::function<void( Game * )> & f )
                        : m_Name( name ),
                          m_Action( f ) {
  // do nothing
}

MenuElement::~MenuElement() {
  // do nothing
}

void MenuElement::Action( Game * g ) const {
  m_Action( g );
}

const std::string & MenuElement::GetName() const {
  return m_Name;
}
