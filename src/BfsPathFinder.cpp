/// Implementation of BfsPathFinder
/**
 * @file BfsPathFinder.cpp
 */

#include <sstream>
#include "BfsPathFinder.h"
#include "CommonFunctions.h"
#include "MovingGameObject.h"

BfsPathFinder::BfsPathFinder( Map * map,
                              const bool & usePortals,
                              const std::pair<int, int> & start,
                              const std::pair<int, int> & end )
                            : m_Map( map ),
                              m_Start( start ),
                              m_End( end ),
                              m_UsePortals( usePortals ) {
  // do nothing
}

const char BfsPathFinder::GetFirstStep( const std::pair<int, int> & start,
                                        const std::pair<int, int> & end ) {
  m_Start = start;
  m_End = end;
  if ( ! Search() ) {
    return 'n';
  }

  std::pair<int, int> parent = m_End;
  std::pair<int, int> child;
  while ( true ) {
    auto it = m_Connections.find( parent );
    if ( it == m_Connections.cend() ) {
      throw MyException( std::string( "Pathfinder error - could not backtrack route" ) );
    }
    child = it->second;
    if ( child == m_Start ) {
      if ( parent.first - m_Start.first == -1 ) {
        return 'w';
      }
      if ( parent.second - m_Start.second == -1 ) {
        return 'a';
      }
      if ( parent.first - m_Start.first == 1 ) {
        return 's';
      }
      if ( parent.second - m_Start.second == 1 ) {
        return 'd';
      }
      throw MyException( std::string( "Pathfinder error - first step is farther than 1" ) );
    }
    parent = child;
  }
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
  if ( m_Map->ValidCoords( newNode ) &&
       ( m_Map->Data()[ newNode.first ][ newNode.second ]->Char() == ' ' ||
         m_Map->Data()[ newNode.first ][ newNode.second ]->Char() == '-' ||
         m_Map->Data()[ newNode.first ][ newNode.second ]->Char() == '*' ||
         m_Map->Data()[ newNode.first ][ newNode.second ]->Char() == 'P' ) &&
       ! Visited( { newNode.first, newNode.second } ) ) {
    m_Queue.push( { newNode.first, newNode.second } );
    PushToConn( newNode, parent );
  }
}

void BfsPathFinder::PushChildrenToQueue( const std::pair<int, int> & n ) {
  // go up
  ValidatePushToQueue( n, { n.first - 1, n.second } );

  // go left
  ValidatePushToQueue( n, { n.first, n.second - 1 } );

  // go down
  ValidatePushToQueue( n, { n.first + 1, n.second } );

  // go right
  ValidatePushToQueue( n, { n.first, n.second + 1  } );
}

const bool BfsPathFinder::Search() {
  Visit( m_Start );
  PushChildrenToQueue( m_Start );
  return SearchRec();
}

const bool BfsPathFinder::SearchRec() {
  if ( m_Queue.empty() ) {
    return false;
  }

  auto n = m_Queue.front();
  Visit( n );
  if ( n == m_End ) {
    return true;
  }
  PushChildrenToQueue( n );
  m_Queue.pop();
  return SearchRec();
}
