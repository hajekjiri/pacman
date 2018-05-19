/// Implementation of Game
/**
 * @file Game.cpp
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "MyException.h"
#include "Game.h"
#include "Ghost.h"
#include "MenuElement.h"
#include "CommonFunctions.cpp"
#include "Blank.h"
#include "BonusCoin.h"
#include "Coin.h"
#include "Portal.h"
#include "Wall.h"

const int Game::STATE_RUNNING = 0;
const int Game::STATE_PAUSED = 1;
const int Game::STATE_MENU = 2;
const int Game::STATE_HELP = 3;

Game::Game()
          : m_GameState( Game::STATE_MENU ),
            m_Menu( 1 ),
            m_Pacman( nullptr ) {
  // do sth
}

Game::~Game() {
  delwin( m_Window );
  delwin( m_PauseWin );
  delete m_Pacman;
}

void Game::LoadCfg( const std::string & pathToCfg ) {
  std::ifstream is;
  is.open( ( "./src/cfg/" + pathToCfg ).data() );
  if ( ! is ) {
    // unable to open file
    throw MyException( std::string( "[Game::LoadCfg@43] Unable to open cfg file" ) );
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
      throw MyException( std::string( "[Game::LoadCfg@69] Syntax error in cfg file '" )
                         + pathToCfg + "'" );
      break;
    }

    // push setting to 'm_Settings'
    m_Settings.insert( { type, value } );
  }
}

void Game::LoadMapFromFile( const std::string & path ) {
  std::ifstream is;
  is.open( ( "./src/cfg/" + path ).data() );
  if ( ! is ) {
    // map file does not exist
    throw MyException( std::string( "[Game::LoadMapFromFile@84] Map file '" )
                       + path + "' does not exist" );
  }
  char c;
  int row = 0;
  int col = 0;
  bool newLine = true;
  while ( ! is.eof() ) {
    is.get( c );
    if ( c == '\n' ) {
      ++row;
      col = 0;
      newLine = true;
      continue;
    }
    GameObject * o;
    bool valid = false;
    if ( c == '-' ) {
      // coin
      o = new Coin();
      valid = true;
    }

    if ( c == '#' ) {
      // wall
      o = new Wall();
      if ( valid ) {
        throw MyException( std::string( "[Game::LoadMapFromFile@111] Invalid character '" ) + c + "' in map @ "
                           + std::to_string( row ) + "," + std::to_string( col ) );
      }
      valid = true;
    }

    if ( c == '*' ) {
      // bonus coin
      o = new BonusCoin();
      if ( valid ) {
        throw MyException( std::string( "[Game::LoadMapFromFile@121] Invalid character '" ) + c + "' in map @ "
                           + std::to_string( row ) + "," + std::to_string( col ) );
      }
      valid = true;
    }

    if ( c == ' ' ) {
      // blank
      o = new Blank();
      if ( valid ) {
        throw MyException( std::string( "[Game::LoadMapFromFile@131] Invalid character '" ) + c + "' in map @ "
                           + std::to_string( row ) + "," + std::to_string( col ) );
      }
      valid = true;
    }

    if ( c >= 'A' && c <= 'C' ) {
      // ghost
      o = new Ghost( c );
      if ( valid ) {
        throw MyException( std::string( "[Game::LoadMapFromFile@141] Invalid character '" ) + c + "' in map @ "
                           + std::to_string( row ) + "," + std::to_string( col ) );
      }
      valid = true;
    }

    if ( c >= '0' && c <= '9' ) {
      // portal
      o = new Portal( c - 48 );
      if ( valid ) {
        throw MyException( std::string( "[Game::LoadMapFromFile@151] Invalid character '" ) + c + "' in map @ "
                           + std::to_string( row ) + "," + std::to_string( col ) );
      }
      valid = true;
    }

    if ( c == 'P' ) {
      // pacman
      o = new Pacman();
      if ( valid ) {
        throw MyException( std::string( "[Game::LoadMapFromFile@161] Invalid character '" ) + c + "' in map @ "
                           + std::to_string( row ) + "," + std::to_string( col ) );
      }
      valid = true;
    }
    if ( ! valid ) {
      throw MyException( std::string( "[Game::LoadMapFromFile@167] Invalid character '" ) + c + "' in map @ "
                         + std::to_string( row ) + "," + std::to_string( col ) );
    }

    if ( newLine ) {
      m_Map.m_Data.push_back( std::vector<GameObject*>() );
      newLine = false;
    }
    ( m_Map.m_Data )[ row ].push_back( o );
    ++col;
  }

  {
    int row = 0;
    auto it = m_Map.m_Data.begin();
    size_t size = it->size();
    for ( ; it != m_Map.m_Data.end(); ++it, ++row ) {
      if ( it->size() != size ) {
        throw MyException( std::string( "[Game::LoadMapFromFile@185] Invalid map. Rows 0 and " )
                           + std::to_string( row )
                           + " are not the same length ( "
                           + std::to_string( size ) + " x "
                           + std::to_string( it->size() ) + " )" );
      }
    }
  }
}

void Game::Init( const std::string & pathToCfg ) {
  curs_set( 0 );
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
  auto it = m_Settings.find( "map" );
  if ( it == m_Settings.cend() ) {
    // map missing in settings.cfg
    throw MyException( std::string( "[Game::Init@211] Map settings missing in cfg file '" )
                       + pathToCfg + "'" );
  }
  LoadMapFromFile( it->second );
  m_MapHeight = m_Map.m_Data.size();
}

void Game::Run() {
  while ( true ) {
    switch ( m_GameState ) {
      case Game::STATE_RUNNING:
        Play();
        break;
      case Game::STATE_PAUSED: {
        nodelay( stdscr, false );
        m_PauseWin = newwin( 11, 30, m_MapHeight + 4, 0 );
        box( m_PauseWin, 0, 0 );
        mvwprintw( m_PauseWin, 2, 2, "Game paused" );
        mvwprintw( m_PauseWin, 4, 2, "* press 'c' to continue" );
        mvwprintw( m_PauseWin, 6, 2, "* press 'm' to go to menu" );
        mvwprintw( m_PauseWin, 8, 2, "* press 'q' to quit" );
        while ( true ) {
          int k = wgetch( m_PauseWin );
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
            try {
              m_Menu.m_Options.at( m_Menu.m_HighlightedIdx ).Action( this );
            } catch ( ... ) {
              // TODO: custom exception
              return;
            }
            break;
          case 'q':
            return;
        }
        break;
      }
      case Game::STATE_HELP: {
        nodelay( stdscr, false );
        WINDOW * w = newwin( 20, 40, 0, 0 );
        box( w, 0, 0 );
        int posY = 3;
        int posX = 3;
        std::string text = "* Movement: arrow keys";
        mvwprintw( w, posY, posX, text.data() );
        posY += 2;
        text = "* Goal: collect all white dots";
        mvwprintw( w, posY, posX, text.data() );
        posY += 2;
        text = "* Run away from ghosts";
        mvwprintw( w, posY, posX, text.data() );
        posY += 2;
        text = "* Press 'm' to go back to menu";
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
    }
  }
}

void Game::Play() {
  nodelay( stdscr, true );
  wrefresh( m_Window );
  m_Map.Draw( m_Window );
  mvprintw( m_MapHeight+1, 0, "Press 'p' to pause the game"  );
  refresh();
  while ( true ) {
    int k = wgetch( m_Window );
    // do sth
    switch ( k ) {
      case 'p':
        ChangeState( Game::STATE_PAUSED );
        return;
        break;

      default:
        break;
    }
  }
  nodelay( stdscr, false );
}

void Game::Reset() {
  // TODO
}

void Game::ChangeState( const int & state ) {
  if ( state < 0 || state > 3 ) {
    throw MyException( std::string( "[Game::ChangeState@335] Invalid game state parameter ( " )
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
      m_Window = newwin( m_MapHeight, 20, 0, 0 );
      break;
  }

  m_GameState = state;
  return;
}
