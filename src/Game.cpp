/// Implementation of Game
/**
 * @file Game.cpp
 */
 #include <cctype>
 #include <cstring>
 #include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "CommonFunctions.h"
#include "Game.h"
#include "GameObject.h"
#include "MenuElement.h"
#include "MovingGameObject.h"
#include "MyException.h"

const int Game::STATE_RUNNING = 0;
const int Game::STATE_PAUSED = 1;
const int Game::STATE_MENU = 2;
const int Game::STATE_HELP = 3;
const int Game::STATE_END = 4;
const int Game::STATE_EXIT = 5;

const char * Game::SETTINGS_FILE = "settings.cfg";

const int Game::MODE_CLASSIC = 0;
const int Game::MODE_SURVIVAL = 1;

const int Game::RESULT_WIN = 1;
const int Game::RESULT_LOSS = 2;

Game::Game()
          : m_GameState( Game::STATE_MENU ),
            m_Window( nullptr ),
            m_PauseWin( nullptr ),
            m_InfoWin( nullptr ),
            m_Menu( 1 ),
            m_Pacman( nullptr ),
            m_Score( 0 ),
            m_Turns( 0 ),
            m_Mode( -1 ),
            m_BonusTurns( 0 ),
            m_RespawnBonusTurnNo( 0 ),
            m_Result( 0 ) {
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
  int line = 0;
  while ( true ) {
    std::string lineStr;
    int pos = is.tellg();
    std::getline( is, lineStr );
    ++line;
    if ( is.eof() ) {
      break;
    }
    strLTrim( lineStr );
    if ( lineStr.data()[ 0 ] == '#' ||
         lineStr.size() == 0 ) {
      // ignore comments and empty lines
      continue;
    } else {

      if ( ! strchr( lineStr.data(), ':' ) ) {
        std::ostringstream oss;
        oss << "Invalid cfg file '" << pathToCfg << "' - syntax error on line "
            << line << std::endl << lineStr;
        is.close();
        throw MyException( oss.str() );
      }
      is.seekg( pos, is.beg );
    }
    std::string type;
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
      std::ostringstream oss;
      oss << "Invalid cfg file '" << pathToCfg << "' - unexpected end of file on line "
          << line << std::endl << lineStr;
      is.close();
      throw MyException( oss.str() );
      break;
    }

    // push setting to 'm_Settings'
    m_Settings.insert( { type, value } );
  }
  is.close();
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
            ChangeState( Game::STATE_EXIT );
            break;
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
        WINDOW * w = newwin( 9, 50,
                             ( m_Map.m_Height - 11 ) / 2,
                             m_Map.m_Width + 10 );
        box( m_PauseWin, 0, 0 );

        std::ostringstream oss, oss2;
        if ( m_Result == Game::RESULT_WIN ) {
          oss << "Congratulations, " << Setting( "username" );
          oss2 << "You won!";
        } else if ( m_Result == Game::RESULT_LOSS ) {
          oss << "Im sorry, " << Setting( "username" );
          oss2 << "You lost :(";
        } else {
          std::ostringstream oss;
          oss << "Undefined result ( " << m_Result << " )";
          throw MyException( oss.str() );
        }
        mvwprintw( w, 2, ( 50 - oss.str().size() ) / 2, oss.str().data() );
        mvwprintw( w, 4, ( 50 - oss2.str().size() ) / 2, oss2.str().data() );
        mvwprintw( w, 6, ( 50 - std::string( "Press any key to continue..." ).size() ) / 2, "Press any key to continue..." );
        box( w , 0, 0 );
        wrefresh( w );
        wgetch( w );
        werase( w );
        wrefresh( w );
        delwin( w );
        ChangeState( Game::STATE_MENU );
        break;
      }
      case Game::STATE_EXIT: {
        return;
      }
    }
  }
}

void Game::Play() {
  m_InfoWin = newwin( 20, 60, m_Map.m_Height + 1, 2 );
  m_Result = 0;
  while ( true ) {
    DrawInfo();
    m_Map.Draw( m_Window );
    wrefresh( m_Window );
    wrefresh( m_InfoWin );

    int k = wgetch( m_Window );

    k = tolower( k );
    if ( k == 'p' ) {
      ChangeState( Game::STATE_PAUSED );
      return;
    }

    if ( k == 'w' || k == 'a' || k == 's' || k == 'd' ) {
      if ( ! m_Pacman->MovePacman( k, *this ) ) {
        continue;
      }
      ++m_Turns;
    } else {
      continue;
    }

    if ( m_BonusTurns > 0 ) {
      --m_BonusTurns;
      m_Pacman->Lethal() = true;
    } else {
      m_Pacman->Lethal() = false;
    }

    m_Map.Draw( m_Window );
    wrefresh( m_Window );

    if ( ! m_Pacman->Alive() ) {
      m_Result = Game::RESULT_LOSS;
      delete m_Pacman;
      m_Pacman = nullptr;
      ChangeState( Game::STATE_END );
      return;
    }

    for ( const auto & ghost : m_Ghosts ) {
      ghost->MoveGhost( *this );
    }

    m_Map.Draw( m_Window );
    wrefresh( m_Window );

    if ( ! m_Pacman->Alive() ) {
      m_Result = Game::RESULT_LOSS;
      ChangeState( Game::STATE_END );
      delete m_Pacman;
      m_Pacman = nullptr;
      return;
    }

    switch ( m_Mode ) {
      case Game::MODE_CLASSIC:
        if ( m_Turns == atoi( Setting( "max_turns_classic" ) ) ) {
          if ( CoinsLeft() ) {
            m_Result = Game::RESULT_LOSS;
          } else {
            m_Result = Game::RESULT_WIN;
          }
          ChangeState( Game::STATE_END );
          return;
        }

        if ( ! CoinsLeft() ) {
          m_Result = Game::RESULT_WIN;
          ChangeState( Game::STATE_END );
          return;
        }
        break;
      case Game::MODE_SURVIVAL:
        if ( m_Turns == atoi( Setting( "max_turns_survival" ) ) ) {
          m_Result = Game::RESULT_WIN;
          ChangeState( Game::STATE_END );
          return;
        }
        break;
      default:
        std::ostringstream oss;
        oss << "Invalid game mode ( " << m_Mode << " )";
        throw MyException( oss.str() );
    }
  }
}

void Game::Reset() {
  // delete bonus coords
  m_BonusCoords.clear();

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
   *   can we delete the map elements themselves
   */
  //m_Map.Data()[ 1 ][ 6 ] = new GameObject( ' ' );
  for ( const auto & outsideElem : m_Map.Data() ) {
    for ( const auto & insideElem : outsideElem ) {
      delete insideElem;
    }
  }


  //delete m_Map.Data()[ 1 ][ 6 ];


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
  m_BonusTurns = 0;
  m_RespawnBonusTurnNo = 0;

  // find map path in settings and re-load it
  auto it = m_Settings.find( "map" );
  if ( it == m_Settings.cend() ) {
    /*
     * Map missing in settings
     * ( impossible since a game is
     *   being played at the moment )
     */
    throw MyException( std::string( "Map path not found in internal settings.\nThis should not have happened" ) );
  }
  m_Map.LoadFromFile( it->second, *this );
}

const bool Game::CoinsLeft() {
  for ( const auto & outsideElem : m_Map.Data() ) {
    for ( const auto & insideElem : outsideElem ) {
      if ( insideElem->Char() == '-' ) {
        return true;
      }
    }
  }
  for ( const auto & elem : m_Ghosts ) {
    if ( elem->Carry() && elem->Carry()->Char() == '-' ) {
      return true;
    }
  }
  return false;
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

int & Game::Mode() {
  return m_Mode;
}

int & Game::BonusTurns() {
  return m_BonusTurns;
}

const int & Game::Turns() const {
  return m_Turns;
}

int & Game::RespawnBonusTurnNo() {
  return m_RespawnBonusTurnNo;
}

std::map<std::string, std::string> & Game::Settings() {
  return m_Settings;
}

std::vector<MovingGameObject*> & Game::Ghosts() {
  return m_Ghosts;
}

MovingGameObject *& Game::Pacman() {
  return m_Pacman;
}

std::vector<std::pair<int, int> > & Game::BonusCoords() {
  return m_BonusCoords;
}

const char * Game::Setting( const std::string & key ) const {
  const auto it = m_Settings.find( key );
  if ( it == m_Settings.cend() ) {
    std::ostringstream oss;
    oss << "Invalid cfg file - could not find '" << key << "' setting";
    throw MyException( oss.str() );
  }
  return it->second.data();
}

void Game::ChangeState( const int & state ) {
  if ( state < 0 || state > 5 ) {
    throw MyException( std::string( "Invalid game state parameter ( " )
                       + std::to_string( state ) + " )" );
  }

  switch ( m_GameState ) {
    case Game::STATE_MENU:
      werase( m_Menu.m_Window );
      wrefresh( m_Menu.m_Window );
      delwin( m_Menu.m_Window );
      m_Menu.m_Window = nullptr;
      break;
    case Game::STATE_PAUSED:
      werase( m_PauseWin );
      wrefresh( m_PauseWin );
      delwin( m_PauseWin );
      m_PauseWin = nullptr;

      if ( state == Game::STATE_MENU ) {
        erase();
        refresh();
        werase( m_Window );
        wrefresh( m_Window );
        delwin( m_Window );
        m_Window = nullptr;
        Reset();
      }
      break;
    case Game::STATE_END:
      erase();
      refresh();
      werase( m_Window );
      wrefresh( m_Window );
      delwin( m_Window );
      m_Window = nullptr;
      Reset();
      break;
  }

  switch ( state ) {
    case Game::STATE_MENU:
      m_Mode = -1;
      m_Menu.Init();
      break;
    case Game::STATE_RUNNING:
      m_Window = newwin( m_Map.m_Height, m_Map.m_Width, 1, 2 );
      break;
  }

  if ( m_GameState == Game::STATE_RUNNING && state != Game::STATE_PAUSED ) {
    werase( m_InfoWin );
    wrefresh( m_InfoWin );
    delwin( m_InfoWin );
    m_InfoWin = nullptr;
  }

  if ( m_GameState == Game::STATE_PAUSED && state != Game::STATE_RUNNING ) {
    werase( m_InfoWin );
    wrefresh( m_InfoWin );
    delwin( m_InfoWin );
    m_InfoWin = nullptr;
  }

  if ( m_GameState == Game::STATE_PAUSED && state == Game::STATE_EXIT ) {
    werase( m_Window );
    wrefresh( m_Window );
    delwin( m_Window );
    m_Window = nullptr;
  }

  m_GameState = state;
  return;
}

void Game::RespawnBonus() {
  for ( const auto & elem : m_BonusCoords ) {
    if ( m_Map.Data()[ elem.first ][ elem.second ]->Char() == 'P' ) {
      m_Pacman->Carry()->Char() = '*';
      continue;
    }

    if ( m_Map.Data()[ elem.first ][ elem.second ]->Char() >= 'A' &&
         m_Map.Data()[ elem.first ][ elem.second ]->Char() <= 'Z') {
      auto it = m_Ghosts.begin();
      for ( ;
            it != m_Ghosts.end();
            ++it ) {
        if ( ( *it )->Char() == m_Map.Data()[ elem.first ][ elem.second ]->Char() ) {
          break;
        }
      }
      if ( ( *it )->Char() != m_Map.Data()[ elem.first ][ elem.second ]->Char() ) {
        std::ostringstream oss;
        oss << "Ghost '" << m_Map.Data()[ elem.first ][ elem.second ]->Char()
            << "' not found in game data";
        throw MyException( oss.str() );
      }
      ( *it )->Carry()->Char() = '*';
      continue;
    }

    m_Map.Data()[ elem.first ][ elem.second ]->Char() = '*';
  }
}

void Game::DrawInfo() {
  werase( m_InfoWin );
  int printPos = 2;
  mvwprintw( m_InfoWin, printPos++, 0, "Press 'p' to pause the game"  );
  std::ostringstream oss;
  oss << "Game mode: ";
  switch ( m_Mode ) {
    case Game::MODE_CLASSIC:
      oss << "Classic";
      break;
    case Game::MODE_SURVIVAL:
      oss << "Survival";
      break;
    default:
      oss.str( "" );
      oss.clear();
      oss << "Invalid game mode ( " << m_Mode << " )";
      throw MyException( oss.str() );
      break;
  }
  mvwprintw( m_InfoWin, printPos++, 0, oss.str().data() );
  oss.str( "" );
  oss.clear();
  oss << "Maximum turns: " << Setting( m_Mode == Game::MODE_CLASSIC ?
                                       "max_turns_classic" : "max_turns_survival" );
  mvwprintw( m_InfoWin, printPos++, 0, oss.str().data() );
  oss.str( "" );
  oss.clear();
  oss << "Turns: " << m_Turns;
  mvwprintw( m_InfoWin, printPos++, 0, oss.str().data() );
  oss.str( "" );
  oss.clear();
  oss << "Score: " << m_Score;
  mvwprintw( m_InfoWin, printPos++, 0, oss.str().data() );
  if ( m_BonusTurns > 0 ) {
    oss.str( "" );
    oss.clear();
    oss << "Pacman can now eat 1 ghost in the next " << m_BonusTurns << " turns";
  } else {
    oss.str( "" );
    oss.clear();
    oss << "Pacman will die on contact with a ghost";
  }
  mvwprintw( m_InfoWin, printPos++, 0, oss.str().data() );
  oss.str( "" );
  oss.clear();
  int when = m_RespawnBonusTurnNo - m_Turns;
  int size;
  if ( when > 0 && ( size = m_BonusCoords.size() ) > 0 ) {
    if ( size > 1 ) {
      oss << "Bonuses ";
    } else {
      oss << "Bonus ";
    }
    oss << "will respawn in " << when << ( when == 1 ? " turn" : " turns" );
    mvwprintw( m_InfoWin, printPos++, 0, oss.str().data() );
  }
}
