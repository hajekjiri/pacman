/// Bfs path finder class
/**
 * @file BfsPathFinder.h
 * class for a ghost->pacman path finder using the bfs algorithm
 */

#include <set>
#include <sstream>
#include <queue>
#include <map>
#include <utility>
#include "CommonFunctions.h"
#include "Game.h"
#include "MyException.h"

#ifndef BFS_PATH_FINDER_H
#define BFS_PATH_FINDER_H

class BfsPathFinder {
  public:
    /// Constructor
    BfsPathFinder( Game * game,
                   const bool & usePortals,
                   const bool & noBlock,
                   const std::pair<int, int> & start = { 0, 0 },
                   const std::pair<int, int> & end = { 0, 0 } );

    /// Get first step of returned path
    const std::pair<char, int> GetFirstStep( const std::pair<int, int> & start,
                                             const std::pair<int, int> & end,
                                             const bool & noBlock = false );

  private:
    struct PairIntCmp {
      const bool operator () ( const std::pair<int, int> & lhs,
                               const std::pair<int, int> & rhs ) const {
        if ( lhs.first != rhs.first ) {
          return lhs.first < rhs.first;
        }
        return lhs.second < rhs.second;
      }
    };

    Game * m_Game;
    std::pair<int, int> m_Start;
    std::pair<int, int> m_End;

    /**
     * Use portals when searching path
     */
    bool m_UsePortals;

    /**
     * When true, ignore moving object collisions
     */
    bool m_NoBlock;

    std::map<std::pair<int, int>, std::pair<int, int>, PairIntCmp> m_Connections;
    std::set<std::pair<int, int>, PairIntCmp> m_Visited;
    std::queue<std::pair<int, int> > m_Queue;

    /// check if object is valid, depending on noBlock setting
    const bool ObjectIsValid( const char & c ) const;

    /// Check if node was visited
    const bool Visited( const std::pair<int, int> & n ) const;

    /// Insert node to visited list
    void Visit( const std::pair<int, int> & n );

    /// push node to queue
    void PushToConn( const std::pair<int, int> & next,
                     const std::pair<int, int> & prev );

    /// validate node and push it to queue
    void ValidatePushToQueue( const std::pair<int, int> & parent,
                              const std::pair<int, int> & n );

    /// push child nodes to queue
    const bool PushChildrenToQueue( const std::pair<int, int> & n );

    /// BFS m_Start => m_End
    /**
     *
     */
    const bool Search();
};

#endif // BFS_PATH_FINDER_H
