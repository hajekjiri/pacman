/// Implementation of BfsPathFinder
/**
 * @file BfsPathFinder.cpp
 */

#include <sstream>
#include "BfsPathFinder.h"
#include "CommonFunctions.h"
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

    //
    if ( ! m_Connections.size() ) {
      //throw MyException( std::string( "No connections" ) );
    }

    for ( const auto & elem : m_Connections ) {
      std::ostringstream oss;
      oss << "Connection: { " << elem.first.first << ", " << elem.first.second
          << " } => { " << elem.second.first << ", " << elem.second.second << " }\n";
      //dumpToFile( oss.str().data() );
    }

    std::ostringstream oss;
    oss << "Didnt find pacman, was looking for { " << m_End.first << ", " << m_End.second << " }";
    throw MyException( std::string( oss.str().data() ) );

    return 'r';
  } else {
    //throw MyException( std::string( "Bfs found Pacman" ) );
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
  auto it = m_Visited.find( n );
  if ( it != m_Visited.cend() ) {
    std::ostringstream oss;
    oss << "Found { " << n.first << ", " << n.second << " } ===> { "
        << it->first << ", " << it->second << " }\n";
    dumpToFile( oss.str().data() );
  }
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
         m_Map->Data()[ n.first ][ n.second ]->Char() == '*' ||
         m_Map->Data()[ n.first ][ n.second ]->Char() == 'P' ) &&
       ! Visited( { n.first, n.second } ) ) {
    m_Queue.push( { n.first, n.second } );
    PushToConn( n, parent );
    std::ostringstream oss;
    oss << "Pushing to queue...\n"
        << "Char: " << m_Map->Data()[ n.first ][ n.second ]->Char() << std::endl
        << "Coords: { " << n.first << ", " << n.second << " }\n"
        << "--------------------------------------------------\n";
    dumpToFile( oss.str().data() );
  }
  std::ostringstream oss;
  oss << "Validation failed, not pushing to queue...\n"
      << "Char: " << m_Map->Data()[ n.first ][ n.second ]->Char() << std::endl
      << "Coords: { " << n.first << ", " << n.second << " }\n"
      << "ValidCoords() => " << ( m_Map->ValidCoords( n ) ? "Check\n" : "Fail\n" )
      << "Visited() => " << ( Visited( { n.first, n.second } ) ? "Fail\n" : "Check\n" )
      << "--------------------------------------------------\n";
  dumpToFile( oss.str().data() );
}

void BfsPathFinder::PushChildrenToQueue( const std::pair<int, int> & n ) {
  std::ostringstream oss;
  oss << "Orig coords: { " << n.first << ", " << n.second << " }\n"
      << "Visited so far:\n";
  for ( const auto & elem : m_Visited ) {
    //oss << "{ " << elem.first << ", " << elem.second << " }\n";
  }
  dumpToFile( oss.str().data() );
  // up
  dumpToFile( "Going up...\n" );
  ValidatePushToQueue( n, { n.first - 1, n.second } );

  // left
  dumpToFile( "Going left...\n" );
  ValidatePushToQueue( n, { n.first, n.second - 1 } );

  // down
  dumpToFile( "Going down...\n" );
  ValidatePushToQueue( n, { n.first + 1, n.second } );

  // right
  dumpToFile( "Going right...\n" );
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
