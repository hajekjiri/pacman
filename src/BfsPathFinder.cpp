/// Implementation of BfsPathFinder
/**
 * @file BfsPathFinder.cpp
 */

#include "BfsPathFinder.h"
#include "MovingGameObject.h"

BfsPathFinder::BfsPathFinder( Map * map,
                              const std::pair<int, int> & start,
                              const std::pair<int, int> & end )
                            : m_Map( map ),
                              m_Start( start ),
                              m_End( end ) {
  // do nothing
}

const char BfsPathFinder::GetFirstStep( const std::pair<int, int> & start,
                                        const std::pair<int, int> & end ) {
  m_Start = start;
  m_End = end;
  if ( ! Search() ) {
    return 'r';
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
                                         const std::pair<int, int> & n ) {
  if ( m_Map->ValidCoords( n ) &&
       ( m_Map->Data()[ n.first ][ n.second ]->Char() == ' ' ||
         m_Map->Data()[ n.first ][ n.second ]->Char() == '-' ||
         m_Map->Data()[ n.first ][ n.second ]->Char() == '*' ) &&
       ! Visited( { n.first, n.second } ) ) {
    m_Queue.push( { n.first, n.second } );
    PushToConn( n, parent );
  }
}

void BfsPathFinder::PushChildrenToQueue( const std::pair<int, int> & n ) {
  // up
  ValidatePushToQueue( n, { n.first - 1, n.second } );

  // left
  ValidatePushToQueue( n, { n.first, n.second - 1 } );

  // down
  ValidatePushToQueue( n, { n.first + 1, n.second } );

  // right
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
