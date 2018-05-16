/// Implementation of MenuElement
/**
 * @file MenuElement.cpp
 */


#include "MenuElement.h"

const int MenuElement::ELEMENT_INVALID = -1;
const int MenuElement::ELEMENT_PLAY = 0;
const int MenuElement::ELEMENT_HELP = 1;
const int MenuElement::ELEMENT_EXIT = 2;

MenuElement::MenuElement( const int & type )
                        : m_Type( type ) {
  switch ( m_Type ) {
    case ELEMENT_PLAY:
      m_Name = "Play";
      break;
    case ELEMENT_HELP:
      m_Name = "Help";
      break;
    case ELEMENT_EXIT:
      m_Name = "Exit";
      break;
    default:
      m_Type = -1;
      m_Name = "Invalid element";
  }
}

MenuElement::~MenuElement() {
  // do nothing
}

const int & MenuElement::GetType() const {
  return m_Type;
}
