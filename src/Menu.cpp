/// Implementation of Menu
/**
 * @file Menu.cpp
 */

#include "Menu.h"

const bool Menu::DIRECTION_UP = false;
const bool Menu::DIRECTION_DOWN = true;

Menu::Menu( const int & preset )
          : m_Preset( preset ),
            m_Window( nullptr ),
            m_HighlightedIdx( -1 ) {
  switch ( m_Preset ) {
    case 1:
      Add( MenuElement( MenuElement::ELEMENT_PLAY ) );
      Add( MenuElement( MenuElement::ELEMENT_HELP ) );
      Add( MenuElement( MenuElement::ELEMENT_EXIT ) );
      break;

    default:
      break;
  }
}

Menu::~Menu() {
  // do nothing
}

void Menu::Add( const MenuElement & e ) {
  m_Options.push_back( e );
  if ( static_cast<int>( m_Options.size() ) == 1 ) {
    m_HighlightedIdx = 0;
  }
}

void Menu::Draw() {
  /*
   * TODO
   * - draw menu on the screen
   */
}

void Menu::Clear() {
  /*
   * TODO
   * - clear menu
   */
}

const int & Menu::GetElem() const {
  if ( m_HighlightedIdx < 0 ||
       m_HighlightedIdx >= static_cast<int>( m_Options.size() ) ) {
    return MenuElement::ELEMENT_INVALID;
  }
  return m_Options[ m_HighlightedIdx ].GetType();
}

void Menu::Move( const bool & direction ) {
  // TODO
}

void Menu::MoveUp() {
  Move( false );
}

void Menu::MoveDown() {
  Move( true );
}
