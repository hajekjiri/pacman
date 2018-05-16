/// Implementation of Game
/**
 * @file Game.cpp
 */

#include "Game.h"

const int Game::STATE_RUNNING = 0;
const int Game::STATE_PAUSED = 1;
const int Game::STATE_MENU = 2;

Game::Game()
          : m_GameState( Game::STATE_MENU ) {
  // do sth
}

Game::~Game() {
  // TODO
}

void Game::Init( const std::string & pathToConfig ) {
  initscr();
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
        break;
      case Game::STATE_PAUSED:
        break;
      case Game::STATE_MENU:
        m_Menu.Draw();
        int k = wgetch( m_Menu.m_Window );
        switch ( k ) {
          case KEY_UP:
            m_Menu.MoveUp();
            mvprintw( 13, 0, "Moved up   " );
            break;
          case KEY_DOWN:
            m_Menu.MoveDown();
            mvprintw( 13, 0, "Moved down   " );
            break;
        }
        break;
    }
  }
}

void Game::ChangeState( const int & state ) {
  // TODO
}
