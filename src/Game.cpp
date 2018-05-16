/// Implementation of Game
/**
 * @file Game.cpp
 */

#include "Game.h"
#include "Ghost.h"
#include "MenuElement.h"

const int Game::STATE_RUNNING = 0;
const int Game::STATE_PAUSED = 1;
const int Game::STATE_MENU = 2;
const int Game::STATE_HELP = 3;

Game::Game()
          : m_GameState( Game::STATE_MENU ) {
  // do sth
}

Game::~Game() {
  delwin( m_Window );
}

void Game::Init( const std::string & pathToConfig ) {
  curs_set( 0 );
  m_Window = newwin( 10, 20, 0, 0 );
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
  // do sth
}

void Game::Run() {
  while ( true ) {
    switch ( m_GameState ) {
      case Game::STATE_RUNNING:
        Play();
        break;
      case Game::STATE_PAUSED:
        // TODO
        break;
      case Game::STATE_MENU: {
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
        werase( m_Menu.m_Window );
        wrefresh( m_Menu.m_Window );
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
        text = "* Press 'q' to go back";
        mvwprintw( w, posY, posX, text.data() );

        while ( true ) {
          int k = wgetch( w );
          if ( k == 'q' ) {
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
  // TODO
}

void Game::ChangeState( const int & state ) {
  if ( state < 0 || state > 3 ) {
    throw 2;
  }
  m_GameState = state;
}
