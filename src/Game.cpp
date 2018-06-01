/// Implementation of Game
/**
 * @file Game.cpp
 */

#include <fstream>
#include <sstream>
#include "CommonFunctions.h"
#include "Game.h"
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
            m_Pacman( nullptr ),
            m_Score( 0 ),
            m_Turns( 0 ),
            m_Mode( -1 ),
            m_BonusTurns( 0 ),
            m_RespawnBonusTurnNo( 0 ),
            m_Result( 0 ) {
  // do nothing
}

Game::~Game() {
  delwin( m_Window );
  delwin( m_PauseWin );
  delwin( m_InfoWin );

  // delete portals
  for ( auto & elem : m_Portals ) {
    delete elem;
  }

  // delete carry objects
  if ( m_Pacman ) {
    delete m_Pacman->GetCarry();
  }

  for ( const auto & elem : m_Ghosts ) {
    if ( elem ) {
      delete elem->GetCarry();
    }
  }

  /*
   * only after deleting carry objects
   * can we delete the map elements themselves
   */
  for ( const auto & outsideElem : m_Map.GetData() ) {
    for ( const auto & insideElem : outsideElem ) {
      delete insideElem;
    }
  }
}

void Game::LoadCfg( const std::string & pathToCfg ) {
  std::ifstream is;
  is.open( ( "./cfg/" + pathToCfg ).data() );
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
      if ( lineStr.find( ':' ) == std::string::npos ) {
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

    Setting::Validate( type, value );

    auto it = m_Settings.find( type );
    if ( it != m_Settings.cend() ) {
      std::ostringstream oss;
      oss << "Invalid cfg file - duplicate setting '" << type << "'";
      throw MyException( oss.str().data() );
    }

    // push setting to 'm_Settings'
    m_Settings.insert( { type, Setting( value ) } );
  }
  is.close();

  if ( m_Settings.size() != 8 ) {
    throw MyException( "Some settings are missing, please check the documentation" );
  }
}

void Game::Init( const std::string & pathToCfg ) {
  m_Menu.Init();
  LoadCfg( pathToCfg );
  m_Map.LoadFromFile( FindSetting( "map" ).GetStrConst(), *this );
}

void Game::Run() {
  while ( true ) {
    switch ( m_GameState ) {
      case Game::STATE_RUNNING:
        Play();
        break;
      case Game::STATE_PAUSED: {
        m_PauseWin = newwin( 11, 30,
                             ( m_Map.GetHeightConst() - 11 ) / 2,
                             m_Map.GetWidthConst() + 10 );
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
        int k = wgetch( m_Menu.GetWindow() );
        switch ( tolower( k ) ) {
          case KEY_UP:
            m_Menu.MoveUp();
            break;
          case 'w':
            m_Menu.MoveUp();
            break;
          case 'k':
            m_Menu.MoveUp();
            break;
          case KEY_DOWN:
            m_Menu.MoveDown();
            break;
          case 's':
            m_Menu.MoveDown();
          case 'h':
            m_Menu.MoveDown();
            break;
          case 10:
              m_Menu.GetOptionsConst().at( m_Menu.GetHighlightedIdxConst() ).GetActionConst( this );
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
                             ( m_Map.GetHeightConst() - 9 ) / 2,
                             m_Map.GetWidthConst() + 6 );
        box( m_PauseWin, 0, 0 );

        std::ostringstream oss, oss2;
        if ( m_Result == Game::RESULT_WIN ) {
          oss << "Congratulations, " << FindSetting( "username" ).GetStrConst();
          oss2 << "You won!";
        } else if ( m_Result == Game::RESULT_LOSS ) {
          oss << "Im sorry, " << FindSetting( "username" ).GetStrConst();
          oss2 << "You lost :(";
        } else {
          std::ostringstream oss;
          oss << "Undefined result ( " << m_Result << " )";
          throw MyException( oss.str() );
        }
        mvwprintw( w, 2, ( 50 - oss.str().size() ) / 2, oss.str().data() );
        mvwprintw( w, 4, ( 50 - oss2.str().size() ) / 2, oss2.str().data() );
        std::string continueMessage = "Press enter to continue...";
        mvwprintw( w, 6, ( 50 - continueMessage.size() ) / 2, continueMessage.data() );
        box( w , 0, 0 );
        wrefresh( w );
        while ( true ) {
          int k = wgetch( w );
          if ( k == 10 ) {
            break;
          }
        }
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
  m_InfoWin = newwin( 9, 41, m_Map.GetHeightConst() + 1, 2 );
  m_Result = 0;
  keypad( m_Window, true );
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

    if ( k == 'w' || k == 'a' || k == 's' || k == 'd' ||
         k == 'k' || k == 'h' || k == 'j' || k == 'l' ||
         k == KEY_UP || k == KEY_LEFT || k == KEY_DOWN || k == KEY_RIGHT ) {
      if ( ! m_Pacman->MovePacman( k, *this ) ) {
        continue;
      }
      ++m_Turns;
    } else {
      continue;
    }

    if ( m_BonusTurns > 0 ) {
      --m_BonusTurns;
      m_Pacman->GetLethal() = true;
    } else {
      m_Pacman->GetLethal() = false;
    }

    m_Map.Draw( m_Window );
    wrefresh( m_Window );

    if ( ! m_Pacman->GetAlive() ) {
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

    if ( ! m_Pacman->GetAlive() ) {
      m_Result = Game::RESULT_LOSS;
      ChangeState( Game::STATE_END );
      delete m_Pacman;
      m_Pacman = nullptr;
      return;
    }

    switch ( m_Mode ) {
      case Game::MODE_CLASSIC:
        if ( m_Turns == FindSetting( "max_turns_classic" ).GetIntConst() ) {
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
        if ( m_Turns == FindSetting( "max_turns_survival" ).GetIntConst() ) {
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
    delete m_Pacman->GetCarry();
  }
  for ( const auto & elem : m_Ghosts ) {
    if ( elem ) {
      delete elem->GetCarry();
    }
  }

  /*
   * only after deleting carry objects
   *   can we delete the map elements themselves
   */
  for ( const auto & outsideElem : m_Map.GetData() ) {
    for ( const auto & insideElem : outsideElem ) {
      delete insideElem;
    }
  }

  m_Ghosts.clear();
  m_Pacman = nullptr;
  for ( auto & elem : m_Ghosts ) {
    elem = nullptr;
  }

  // reset Map's data vector
  m_Map.GetData().clear();

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
  m_Map.LoadFromFile( it->second.GetStrConst(), *this );
}

const bool Game::CoinsLeft() {
  for ( const auto & outsideElem : m_Map.GetData() ) {
    for ( const auto & insideElem : outsideElem ) {
      if ( insideElem->GetChar() == '-' ) {
        return true;
      }
    }
  }
  for ( const auto & elem : m_Ghosts ) {
    if ( elem->GetCarry() && elem->GetCarry()->GetChar() == '-' ) {
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

std::vector<Portal*> & Game::GetPortals() {
  return m_Portals;
}

int & Game::GetScore() {
  return m_Score;
}

int & Game::GetMode() {
  return m_Mode;
}

int & Game::GetBonusTurns() {
  return m_BonusTurns;
}

int & Game::GetRespawnBonusTurnNo() {
  return m_RespawnBonusTurnNo;
}

const int & Game::GetTurnsConst() const {
  return m_Turns;
}

std::map<std::string, Setting> & Game::GetSettings() {
  return m_Settings;
}

std::vector<MovingGameObject*> & Game::GetGhosts() {
  return m_Ghosts;
}

MovingGameObject *& Game::GetPacman() {
  return m_Pacman;
}

std::vector<std::pair<int, int> > & Game::GetBonusCoords() {
  return m_BonusCoords;
}

const Setting & Game::FindSetting( const std::string & key ) const {
  const auto it = m_Settings.find( key );
  if ( it == m_Settings.cend() ) {
    std::ostringstream oss;
    oss << "Could not find '" << key << "' setting";
    throw MyException( oss.str() );
  }
  return it->second;
}

void Game::ChangeState( const int & state ) {
  if ( state < 0 || state > 5 ) {
    throw MyException( std::string( "Invalid game state parameter ( " )
                       + std::to_string( state ) + " )" );
  }

  switch ( m_GameState ) {
    case Game::STATE_MENU:
      werase( m_Menu.GetWindow() );
      wrefresh( m_Menu.GetWindow() );
      delwin( m_Menu.GetWindow() );
      m_Menu.GetWindow() = nullptr;
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
        werase( m_InfoWin );
        wrefresh( m_InfoWin );
        delwin( m_InfoWin );
        m_InfoWin = nullptr;
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
      werase( m_InfoWin );
      wrefresh( m_InfoWin );
      delwin( m_InfoWin );
      m_InfoWin = nullptr;
      Reset();
      break;
  }

  switch ( state ) {
    case Game::STATE_MENU:
      m_Mode = -1;
      m_Menu.Init();
      break;
    case Game::STATE_RUNNING:
      m_Window = newwin( m_Map.GetHeightConst(), m_Map.GetWidthConst(), 1, 2 );
      break;
    case Game::STATE_END:
      wrefresh( m_Window );
      DrawInfo();
      break;
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
    if ( m_Map.GetData()[ elem.first ][ elem.second ]->GetChar() == 'P' ) {
      m_Pacman->GetCarry()->GetChar() = '*';
      continue;
    }

    if ( m_Map.GetData()[ elem.first ][ elem.second ]->GetChar() >= 'A' &&
         m_Map.GetData()[ elem.first ][ elem.second ]->GetChar() <= 'Z') {
      auto it = m_Ghosts.begin();
      for ( ;
            it != m_Ghosts.end();
            ++it ) {
        if ( ( *it )->GetChar() == m_Map.GetData()[ elem.first ][ elem.second ]->GetChar() ) {
          break;
        }
      }
      if ( ( *it )->GetChar() != m_Map.GetData()[ elem.first ][ elem.second ]->GetChar() ) {
        std::ostringstream oss;
        oss << "Ghost '" << m_Map.GetData()[ elem.first ][ elem.second ]->GetChar()
            << "' not found in game data";
        throw MyException( oss.str() );
      }
      ( *it )->GetCarry()->GetChar() = '*';
      continue;
    }

    m_Map.GetData()[ elem.first ][ elem.second ]->GetChar() = '*';
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
  oss << "Maximum turns: "
      << FindSetting( m_Mode == Game::MODE_CLASSIC ?
                     "max_turns_classic" : "max_turns_survival" ).GetStrConst();
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
      oss << "Bonus coins";
    } else {
      oss << "Bonus coin";
    }
    oss << " will respawn in " << when << ( when == 1 ? " turn" : " turns" );
    mvwprintw( m_InfoWin, printPos++, 0, oss.str().data() );
  }
}
