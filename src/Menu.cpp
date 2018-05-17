/// Implementation of Menu
/**
 * @file Menu.cpp
 */

#include "Menu.h"
#include "MenuElement.h"

const bool Menu::DIRECTION_UP = false;
const bool Menu::DIRECTION_DOWN = true;

Menu::Menu( const int & preset )
          : m_Preset( preset ),
            m_Window( nullptr ),
            m_HighlightedIdx( -1 ) {
  switch ( m_Preset ) {
    case 1:
      Add( MenuElement( "Play",
                         []( Game * g ) {
                           g->ChangeState( Game::STATE_RUNNING );
                         } ) );
      Add( MenuElement( "Reload cfg",
                        []( Game * g ) {

                        } ) );
      Add( MenuElement( "Help",
                        []( Game * g ) {
                          g->ChangeState( Game::STATE_HELP );
                        } ) );
      Add( MenuElement( "Exit",
                        []( Game * g ) {
                          throw 1;
                        } ) );
      break;

    default:
      break;
  }
}

Menu::~Menu() {
  delwin( m_Window );
}

void Menu::Init() {
  m_Window = newwin( ( 3 * m_Options.size() ) + 2,
                     20, 4, 10 );
  keypad( m_Window, true );
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
   werase( m_Window );
   box( m_Window, 0, 0 );

   int index = 0;
   int posY = 2;
   for ( const MenuElement & elem : m_Options ) {
     int posX = ( 20 - elem.m_Name.size() ) / 2;
     if ( index++ == m_HighlightedIdx ) {
       posX -= 2;
       wattron( m_Window, A_REVERSE );
       mvwprintw( m_Window, posY, posX, ( "[ " + elem.m_Name + " ]" ).data() );
     } else {
       mvwprintw( m_Window, posY, posX, elem.m_Name.data() );
     }
     wattroff( m_Window, A_REVERSE );
     wmove( m_Window, 0, 0 );
     posY += 3;
   }
   wrefresh( m_Window );
}

void Menu::Clear() {
  /*
   * TODO
   * - clear menu
   */
  werase( m_Window );
}

void Menu::Move( const bool & direction ) {
  switch( direction ) {
    case Menu::DIRECTION_UP:
      if ( m_HighlightedIdx == 0 ) {
        m_HighlightedIdx = static_cast<int>( m_Options.size() - 1 );
      } else {
        --m_HighlightedIdx;
      }
      break;
    case Menu::DIRECTION_DOWN:
      if ( m_HighlightedIdx == static_cast<int>( m_Options.size() - 1 ) ) {
        m_HighlightedIdx = 0;
      } else {
        ++m_HighlightedIdx;
      }
      break;
  }
}

void Menu::MoveUp() {
  Move( false );
}

void Menu::MoveDown() {
  Move( true );
}
