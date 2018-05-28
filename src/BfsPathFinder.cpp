/// Implementation of BfsPathFinder
/**
 * @file BfsPathFinder.cpp
 */

#include <cstdlib>
#include <sstream>
#include <utility>
#include "BfsPathFinder.h"
#include "CommonFunctions.h"
#include "MovingGameObject.h"

BfsPathFinder::BfsPathFinder( Game * game,
                              const bool & usePortals,
                              const bool & noBlock,
                              const std::pair<int, int> & start,
                              const std::pair<int, int> & end )
                            : m_Game( game ),
                              m_Start( start ),
                              m_End( end ),
                              m_UsePortals( usePortals ),
                              m_NoBlock( noBlock ) {
  // do nothing
}

const std::pair<char, int> BfsPathFinder::GetFirstStep( const std::pair<int, int> & start,
                                                        const std::pair<int, int> & end,
                                                        const bool & noBlock ) {
  m_Start = start;
  m_End = end;
  m_NoBlock = noBlock;
  bool pathExists = Search();
  m_NoBlock = false;

  if ( ! pathExists ) {
    return { 'n', -1 };
  }

  std::pair<int, int> parent = m_End;
  std::pair<int, int> child;
  int distance = 0;
  while ( true ) {
    auto it = m_Connections.find( parent );
    if ( it == m_Connections.cend() ) {
      throw MyException( std::string( "Pathfinder error - could not backtrack route" ) );
    }
    ++distance;
    child = it->second;
    if ( child == m_Start ) {
      if ( parent.first - m_Start.first == -1 ) {
        return { 'w', distance } ;
      }
      if ( parent.second - m_Start.second == -1 ) {
        return { 'a', distance };
      }
      if ( parent.first - m_Start.first == 1 ) {
        return { 's', distance };
      }
      if ( parent.second - m_Start.second == 1 ) {
        return { 'd', distance };
      }
      std::ostringstream oss;
      oss << "Pathfinder error - first step is farther than 1\n"
          << "{ " << parent.first << ", " << parent.second << " } => "
          << "{ " << child.first << ", " << child.second << " }\n";
      throw MyException( oss.str() );
    }
    parent = child;
  }
}

const bool BfsPathFinder::ObjectIsValid( const char & c ) const {
  if ( c == ' ' || c == '-' || c == '*' || c == 'P' ) {
    return true;
  }
  if ( m_NoBlock && isGhost( c ) ) {
    return true;
  }
  if ( m_UsePortals && c >= '0' && c <= '9' ) {
    return true;
  }
  return false;
}

const bool BfsPathFinder::Visited( const std::pair<int, int> & n ) const {
  return m_Visited.find( n ) != m_Visited.cend();
}

void BfsPathFinder::Visit( const std::pair<int, int> & n ) {
  m_Visited.insert( n );
}

void BfsPathFinder::PushToConn( const std::pair<int, int> & next,
                                const std::pair<int, int> & prev ) {
  m_Connections.insert( { next, prev } );
}

void BfsPathFinder::ValidatePushToQueue( const std::pair<int, int> & parent,
                                         const std::pair<int, int> & newNode ) {

  /*
   * if coords are not out of range
   *  and ( object is blank, coin, or bonus )
   *  and ( noBlock is ON or object is non-ghost )
   *  and ( node not visited )
   * => push to queue
   */
  if ( m_Game->GetMap().ValidCoords( newNode ) &&
       ! Visited( { newNode.first, newNode.second } ) &&
       ObjectIsValid( m_Game->GetMap().Data()[ newNode.first ][ newNode.second ]->Char() ) ) {
    m_Queue.push( { newNode.first, newNode.second } );
    PushToConn( newNode, parent );
    Visit( newNode );
  }
}

const bool BfsPathFinder::PushChildrenToQueue( const std::pair<int, int> & n ) {
  if ( m_UsePortals && n != m_Start ) {
    // check if node is a portal
    bool found = false;
    std::pair<int, int> pairCoords;
    for ( const auto & elem : m_Game->Portals() ) {
      if ( elem->Coords() == n ) {
        found = true;
        pairCoords = elem->PairCoords();
        break;
      }
    }
    // if found, go through portal
    if ( found ) {
      if ( ( isGhost( m_Game->GetMap().Data()[ pairCoords.first ][ pairCoords.second ]->Char() ) ||
           m_Game->GetMap().Data()[ pairCoords.first ][ pairCoords.second ]->Char() == 'P' ) &&
           isNextToInLine( n, m_Start ) ) {
        return false;
      }
      PushToConn( pairCoords, n );
      Visit( pairCoords );
      if ( pairCoords == m_End ) {
        return true;
      }
      ValidatePushToQueue( pairCoords, { pairCoords.first - 1, pairCoords.second } );
      ValidatePushToQueue( pairCoords, { pairCoords.first, pairCoords.second - 1 } );
      ValidatePushToQueue( pairCoords, { pairCoords.first + 1, pairCoords.second } );
      ValidatePushToQueue( pairCoords, { pairCoords.first, pairCoords.second + 1  } );
      return false;
    }
  }

  if ( std::abs( m_End.first - n.first ) > std::abs( m_End.second - n.second ) ) {
    // go up
    ValidatePushToQueue( n, { n.first - 1, n.second } );

    // go down
    ValidatePushToQueue( n, { n.first + 1, n.second } );

    // go left
    ValidatePushToQueue( n, { n.first, n.second - 1 } );

    // go right
    ValidatePushToQueue( n, { n.first, n.second + 1  } );
  } else {
    // go left
    ValidatePushToQueue( n, { n.first, n.second - 1 } );

    // go right
    ValidatePushToQueue( n, { n.first, n.second + 1  } );

    // go up
    ValidatePushToQueue( n, { n.first - 1, n.second } );

    // go down
    ValidatePushToQueue( n, { n.first + 1, n.second } );
  }


  return false;
}

const bool BfsPathFinder::Search() {
  if ( m_Start == m_End ) {
    return false;
  }

  // clear containers
  m_Connections.clear();
  m_Queue = std::queue<std::pair<int, int> >();
  m_Visited.clear();

  Visit( m_Start );
  if ( PushChildrenToQueue( m_Start ) ) {
      return true;
  }

  while ( ! m_Queue.empty() ) {
    auto n = m_Queue.front();

    if ( n == m_End ) {
      return true;
    }

    if ( PushChildrenToQueue( n ) ) {
      return true;
    }
    m_Queue.pop();
  }
  return false;
}
