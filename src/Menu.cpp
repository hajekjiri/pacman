/// Implementation of Menu
/**
 * @file Menu.cpp
 */

#include "Menu.h"
#include "Game.h"

const bool Menu::DIRECTION_UP = false;
const bool Menu::DIRECTION_DOWN = true;

Menu::Menu()
          : m_Window( nullptr ),
            m_HighlightedIdx( -1 ) {
  Add( MenuElement( "Classic mode",
                     []( Game * g ) {
                       g->ChangeState( Game::STATE_RUNNING );
                       g->GetMode() = Game::MODE_CLASSIC;
                     } ) );
  Add( MenuElement( "Survival mode",
                     []( Game * g ) {
                       g->ChangeState( Game::STATE_RUNNING );
                       g->GetMode() = Game::MODE_SURVIVAL;
                     } ) );
  Add( MenuElement( "Reload cfg",
                     []( Game * g ) {
                       g->GetMenu().Clear();
                       WINDOW * w = newwin( 5, 40, 0, 0 );
                       g->GetSettings().clear();
                       g->LoadCfg( Game::SETTINGS_FILE );
                       g->GetMap().LoadFromFile( g->FindSetting( "map" ).GetStrConst(), *g );
                       mvwprintw( w, 2, 4, "Cfg reloaded!" );
                       mvwprintw( w, 4, 4, "Press any key to continue..." );
                       wrefresh( w );
                       wgetch( w );
                       werase( w );
                       wrefresh( w );
                       delwin( w );
                       g->GetMenu().Draw();
                     } ) );
  Add( MenuElement( "Help",
                    []( Game * g ) {
                      g->ChangeState( Game::STATE_HELP );
                    } ) );
  Add( MenuElement( "Exit",
                    []( Game * g ) {
                      g->ChangeState( Game::STATE_EXIT );
                    } ) );
}

Menu::~Menu() {
  delwin( m_Window );
}

void Menu::Init() {
  m_Window = newwin( ( 3 * m_Options.size() ) + 2,
                     26, 4, 10 );
  keypad( m_Window, true );
}

void Menu::Add( const MenuElement & e ) {
  m_Options.push_back( e );
  if ( static_cast<int>( m_Options.size() ) == 1 ) {
    m_HighlightedIdx = 0;
  }
}

void Menu::Draw() {
   werase( m_Window );
   box( m_Window, 0, 0 );

   int index = 0;
   int posY = 2;
   for ( const MenuElement & elem : m_Options ) {
     int posX = ( 26 - elem.GetNameConst().size() ) / 2;
     if ( index++ == m_HighlightedIdx ) {
       posX -= 2;
       wattron( m_Window, A_REVERSE );
       mvwprintw( m_Window, posY, posX, ( "[ " + elem.GetNameConst() + " ]" ).data() );
     } else {
       mvwprintw( m_Window, posY, posX, elem.GetNameConst().data() );
     }
     wattroff( m_Window, A_REVERSE );
     wmove( m_Window, 0, 0 );
     posY += 3;
   }
   wrefresh( m_Window );
}

void Menu::Clear() {
  werase( m_Window );
  wrefresh( m_Window );
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

WINDOW *& Menu::GetWindow() {
  return m_Window;
}

const std::vector<MenuElement> & Menu::GetOptionsConst() const {
  return m_Options;
}

const int & Menu::GetHighlightedIdxConst() {
  return m_HighlightedIdx;
}
