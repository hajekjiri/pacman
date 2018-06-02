/// Implementation of BfsPathFinder
/**
 * @file BfsPathFinder.cpp
 */

#include <sstream>
#include "BfsPathFinder.h"
#include "CommonFunctions.h"
#include "MovingGameObject.h"
#include "MyException.h"

BfsPathFinder::BfsPathFinder( Game * const game,
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

BfsPathFinder::~BfsPathFinder() {
  // do nothing
}

const std::pair<char, int> BfsPathFinder::GetFirstStep( const std::pair<int, int> & start,
                                                        const std::pair<int, int> & end,
                                                        const bool & noBlock ) {
  // set member variables
  m_Start = start;
  m_End = end;
  m_NoBlock = noBlock;
  bool pathExists = Search();
  m_NoBlock = false;

  // end if no path found
  if ( ! pathExists ) {
    return { 'n', -1 };
  }

  /*
   * backtrack the path using stored connections,
   *  every connection has no more than 1 parent
   */
  std::pair<int, int> parent = m_End;
  std::pair<int, int> child;
  int distance = 0;
  while ( true ) {
    auto it = m_Connections.find( parent );
    if ( it == m_Connections.cend() ) {
      throw MyException( "Pathfinder error - could not backtrack route" );
    }
    ++distance;
    child = it->second;

    /*
     * when we find the starting point,
     *  return the appropriate step ( direction )
     */
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
  /*
   * if the object is Pacman who is sitting on a portal,
   *  and your target is a bonus coin, do not go through Pacman
   */
  if ( c == 'P' &&
       m_Game->GetPacman()->GetCarry()->GetChar() >= '0' &&
       m_Game->GetPacman()->GetCarry()->GetChar() <= '9' &&
       m_Game->GetMap().GetData()[ m_End.first ][ m_End.second ]->GetChar() == '*' ) {
    return false;
  }

  /*
   * if the object is blank, coin, bonus coin, or Pacman,
   *  you are allowed to go through it
   */
  if ( c == ' ' || c == '-' || c == '*' || c == 'P' ) {
    return true;
  }

  /*
   * if noblock is on, and the object is a ghost,
   *  you are allowed to go through it
   */
  if ( m_NoBlock && isGhost( c ) ) {
    return true;
  }

  /*
   * if portals are allowed,
   *  you can use them
   */
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
   * if ( coords are not out of range )
   *  and ( node not visited )
   *  and ( object is valid )
   * => push to queue, push to conections, visit
   */
  if ( m_Game->GetMap().ValidCoords( newNode ) &&
       ! Visited( { newNode.first, newNode.second } ) &&
       ObjectIsValid( m_Game->GetMap().GetData()[ newNode.first ][ newNode.second ]->GetChar() ) ) {
    m_Queue.push( { newNode.first, newNode.second } );
    PushToConn( newNode, parent );
    Visit( newNode );
  }
}

const bool BfsPathFinder::PushChildrenToQueue( const std::pair<int, int> & n ) {
  // if portals are allowed and object is not the starting point
  if ( m_UsePortals && n != m_Start ) {
    // check if node is a portal
    bool found = false;
    std::pair<int, int> pairCoords;
    for ( const auto & elem : m_Game->GetPortals() ) {
      if ( elem->GetCoordsConst() == n ) {
        found = true;
        pairCoords = elem->GetPairCoords();
        break;
      }
    }
    // if found, use portal
    if ( found ) {
      /*
       * if something's at the other end of a portal,
       *  and you are right next to it
       *  -> you cannot use it
       */
      if ( ( isGhost( m_Game->GetMap().GetData()[ pairCoords.first ][ pairCoords.second ]->GetChar() ) ||
           m_Game->GetMap().GetData()[ pairCoords.first ][ pairCoords.second ]->GetChar() == 'P' ) &&
           isNextToInLine( n, m_Start ) ) {
        return false;
      }

      // return if target is at the other end of the portal
      PushToConn( pairCoords, n );
      Visit( pairCoords );
      if ( pairCoords == m_End ) {
        return true;
      }

      /*
       * - handle portal in a different way
       * - we can't call PushChildrenToQueue on pair portal here,
       *    because we would fall into an infinite loop where the
       *    portals would be calling PushChildrenToQueue on each other
       */
      ValidatePushToQueue( pairCoords, { pairCoords.first - 1, pairCoords.second } );
      ValidatePushToQueue( pairCoords, { pairCoords.first, pairCoords.second - 1 } );
      ValidatePushToQueue( pairCoords, { pairCoords.first + 1, pairCoords.second } );
      ValidatePushToQueue( pairCoords, { pairCoords.first, pairCoords.second + 1  } );
      return false;
    }
  }

  /*
   * - if object is father away on the Y axis
   *    than on the X axis, go up/down first
   * - this results in diagonal paths in open space
   */
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
    // otherwise go left/right first

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
  // if start == end, there is no path
  if ( m_Start == m_End ) {
    return false;
  }

  // clear containers
  m_Connections.clear();
  m_Queue = std::queue<std::pair<int, int> >();
  m_Visited.clear();

  // visit start and push its children to queue
  Visit( m_Start );
  if ( PushChildrenToQueue( m_Start ) ) {
      return true;
  }

  /*
   * while queue not empty,
   *  take element from queue and expand on it
   */
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
