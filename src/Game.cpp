/// Implementation of Game
/**
 * @file Game.cpp
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cctype>
#include "MyException.h"
#include "Game.h"
#include "MenuElement.h"
#include "GameObject.h"
#include "MovingGameObject.h"
#include "CommonFunctions.cpp"

const int Game::STATE_RUNNING = 0;
const int Game::STATE_PAUSED = 1;
const int Game::STATE_MENU = 2;
const int Game::STATE_HELP = 3;
const int Game::STATE_END = 4;
const int Game::STATE_EXIT = 5;
const char * Game::SETTINGS_FILE = "settings.cfg";

Game::Game()
          : m_GameState( Game::STATE_MENU ),
            m_Menu( 1 ),
            m_Pacman( nullptr ),
            m_Score( 0 ),
            m_Turns( 0 ) {
  // do sth
}

Game::~Game() {
  delwin( m_Window );
  delwin( m_PauseWin );

  // delete portals
  for ( auto & elem : m_Portals ) {
    delete elem;
  }

  // delete carry objects
  if ( m_Pacman ) {
    delete m_Pacman->Carry();
  }

  for ( const auto & elem : m_Ghosts ) {
    if ( elem ) {
      delete elem->Carry();
    }
  }

  /*
   * only after deleting carry objects
   * can we delete the map elements themselves
   */
  for ( const auto & outsideElem : m_Map.Data() ) {
    for ( const auto & insideElem : outsideElem ) {
      delete insideElem;
    }
  }
}

void Game::LoadCfg( const std::string & pathToCfg ) {
  std::ifstream is;
  is.open( ( "./src/cfg/" + pathToCfg ).data() );
  if ( ! is ) {
    // unable to open file
    throw MyException( std::string( "Unable to open cfg file" ) );
  }
  while ( true ) {
    std::string type;
    int pos = is.tellg();
    std::getline( is, type );
    if ( is.eof() ) {
      break;
    }
    strLTrim( type );
    if  ( ( type.data() )[ 0 ] == '#' ) {
      // line is a comment
      continue;
    } else {
      is.seekg( pos, is.beg );
    }
    std::getline( is, type, ':' );
    strTrim( type );
    if ( is.eof() ) {
      break;
    }
    std::string value;
    std::getline( is, value );
    strTrim( value );
    if ( is.eof() ) {
      // syntax error
      throw MyException( std::string( "Syntax error in cfg file '" )
                         + pathToCfg + "'" );
      break;
    }

    // push setting to 'm_Settings'
    m_Settings.insert( { type, value } );
  }
}

void Game::Init( const std::string & pathToCfg ) {
  m_Menu.Init();
  /*
   * TODO:
   * - parse file @ 'pathToConfig'
   * - apply settings
   * ie:
   * - ghost & pacman settings
   * - ghost & pacman coords
   * - load map file from some 'pathToMap' attribute
   */
  LoadCfg( pathToCfg );
  const char * pathToMap = Setting( "map" );
  if ( ! pathToMap ) {
    // map missing in settings.cfg
    throw MyException( std::string( "Map path not found in cfg file '" )
                       + pathToCfg + "'" );
  }
  m_Map.LoadFromFile( pathToMap, *this );
}

void Game::Run() {
  while ( true ) {
    switch ( m_GameState ) {
      case Game::STATE_RUNNING:
        Play();
        break;
      case Game::STATE_PAUSED: {
        m_PauseWin = newwin( 11, 30,
                             ( m_Map.m_Height - 11 ) / 2,
                             m_Map.m_Width + 10 );
        box( m_PauseWin, 0, 0 );
        mvwprintw( m_PauseWin, 2, 2, "Game paused" );
        mvwprintw( m_PauseWin, 4, 2, "* press 'c' to continue" );
        mvwprintw( m_PauseWin, 6, 2, "* press 'm' to go to menu" );
        mvwprintw( m_PauseWin, 8, 2, "* press 'q' to quit" );
        wrefresh( m_PauseWin );
        while ( true ) {
          int k = getch();
          if ( k == 'c' ) {
            ChangeState( Game::STATE_RUNNING );
            break;
          }
          if ( k == 'm' ) {
            ChangeState( Game::STATE_MENU );
            break;
          }
          if ( k == 'q' ) {
            return;
          }
        }
        break;
      }
      case Game::STATE_MENU: {
        nodelay( stdscr, false );
        m_Menu.Draw();
        int k = wgetch( m_Menu.m_Window );
        switch ( k ) {
          case KEY_UP:
            m_Menu.MoveUp();
            break;
          case KEY_DOWN:
            m_Menu.MoveDown();
            break;
          case 10:
              m_Menu.m_Options.at( m_Menu.m_HighlightedIdx ).Action( this );
            break;
        }
        break;
      }
      case Game::STATE_HELP: {
        nodelay( stdscr, false );
        WINDOW * w = newwin( 20, 60, 0, 0 );
        box( w, 0, 0 );
        int posY = 3;
        int posX = 3;
        std::string text = "* Movement: arrow keys";
        mvwprintw( w, posY, posX, text.data() );
        posY += 2;
        text = "* Goal: collect all coins ( '-' )";
        mvwprintw( w, posY, posX, text.data() );
        posY += 2;
        text = "* Run away from ghosts ( [A-Z]-{P} )";
        mvwprintw( w, posY, posX, text.data() );
        posY += 2;
        text = "* Press 'm' to go back to menu ( 'm' )";
        mvwprintw( w, posY, posX, text.data() );

        while ( true ) {
          int k = wgetch( w );
          if ( k == 'm' ) {
            werase( w );
            wrefresh( w );
            delwin( w );
            ChangeState( STATE_MENU );
            break;
          }
        }

        break;
      }
      case Game::STATE_END: {
        WINDOW * w = newwin( 11, 30,
                             ( m_Map.m_Height - 11 ) / 2,
                             m_Map.m_Width + 10 );
        box( m_PauseWin, 0, 0 );

        mvwprintw( w, 2, 12, m_Pacman->Alive() ? "You won" : "You lost" );
        mvwprintw( w, 4, 12, "press any key to continue" );
        getch();
        werase( w );
        delwin( w );
        ChangeState( Game::STATE_MENU );
        return;
      }
      case Game::STATE_EXIT: {
        return;
      }
    }
  }
}

void Game::Play() {
  mvprintw( m_Map.m_Height + 1, 0, "Press 'p' to pause the game"  );
  while ( true ) {
    // print score & number of turns
    std::ostringstream oss;
    oss << "Score: " << m_Score;
    mvprintw( m_Map.m_Height + 2, 0, oss.str().data() );
    oss.str( "" );
    oss.clear();
    oss << "Turns: " << m_Turns;
    mvprintw( m_Map.m_Height + 3, 0, oss.str().data() );
    refresh();

    m_Map.Draw( m_Window );
    wrefresh( m_Window );


    int k = getch();
    k = tolower( k );
    if ( k == 'p' ) {
      ChangeState( Game::STATE_PAUSED );
      return;
    }

    if ( k == 'w' || k == 'a' || k == 's' || k == 'd' ) {
      m_Map.CheckSize();
      m_Pacman->Move( k, *this );
      ++m_Turns;
      if ( ! m_Pacman->Alive() ) {
        ChangeState( Game::STATE_END );
        return;
      }
    }
  }
}

void Game::Reset() {
  // delete portals
  for ( auto & elem : m_Portals ) {
    delete elem;
  }
  m_Portals.clear();

  // delete carry objects
  if ( m_Pacman ) {
    delete m_Pacman->Carry();
  }
  for ( const auto & elem : m_Ghosts ) {
    if ( elem ) {
      delete elem->Carry();
    }
  }

  /*
   * only after deleting carry objects
   * can we delete the map elements themselves
   */
  for ( const auto & outsideElem : m_Map.Data() ) {
    for ( const auto & insideElem : outsideElem ) {
      delete insideElem;
    }
  }

  // reset vector
  m_Ghosts.clear();

  m_Pacman = nullptr;
  for ( auto & elem : m_Ghosts ) {
    elem = nullptr;
  }

  // reset Map's data vector
  m_Map.Data().clear();

  // reset score & turns
  m_Score = 0;
  m_Turns = 0;

  // find map path in settings and re-load it
  auto it = m_Settings.find( "map" );
  if ( it == m_Settings.cend() ) {
    /*
     * Map missing in settings
     * ( impossible since a game is
     *   being played at the moment )
     */
    throw MyException( std::string( "Map path not found in settings.\nThis should not have happened" ) );
  }
  m_Map.LoadFromFile( it->second, *this );
}

Map & Game::GetMap() {
  return m_Map;
}

Menu & Game::GetMenu() {
  return m_Menu;
}

std::vector<Portal*> & Game::Portals() {
  return m_Portals;
}

int & Game::Score() {
  return m_Score;
}

const char * Game::Setting( const std::string & key ) const {
  const auto it = m_Settings.find( key );
  if ( it == m_Settings.cend() ) {
    return nullptr;
  }
  return it->second.data();
}

void Game::ChangeState( const int & state ) {
  if ( state < 0 || state > 3 ) {
    throw MyException( std::string( "Invalid game state parameter ( " )
                       + std::to_string( state ) + " )" );
  }

  switch ( m_GameState ) {
    case Game::STATE_MENU:
      werase( m_Menu.m_Window );
      wrefresh( m_Menu.m_Window );
      delwin( m_Menu.m_Window );
      break;
    case Game::STATE_PAUSED:
      werase( m_PauseWin );
      wrefresh( m_PauseWin );
      delwin( m_PauseWin );

      if ( state == Game::STATE_MENU ) {
        erase();
        refresh();
        werase( m_Window );
        wrefresh( m_Window );
        delwin( m_Window );
      }

      break;
  }

  switch ( state ) {
    case Game::STATE_MENU:
      m_Menu.Init();
      break;
    case Game::STATE_RUNNING:
      m_Window = newwin( m_Map.m_Height, m_Map.m_Width, 0, 0 );
      break;
  }

  if ( m_GameState == Game::STATE_PAUSED && state == Game::STATE_MENU ) {
    Reset();
  }

  m_GameState = state;
  return;
}
