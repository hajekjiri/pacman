/// Bfs path finder class
/**
 * @file BfsPathFinder.h
 */

#include <map>
#include <queue>
#include <set>
#include <utility>
#include "Game.h"

#ifndef BFS_PATH_FINDER_H
#define BFS_PATH_FINDER_H

/// Path finder using the bfs algorithm
class BfsPathFinder {
  public:
    /**
     * @brief Constructor
     * @param game Game for which to find the path
     * @param usePortals Boolean value, true => use portals while searching for path
     * @param start Start coordinates
     * @param end End coordinates
     */
    BfsPathFinder( Game * game,
                   const bool & usePortals,
                   const bool & noBlock,
                   const std::pair<int, int> & start = { 0, 0 },
                   const std::pair<int, int> & end = { 0, 0 } );

    /// Destructor
    ~BfsPathFinder();

    /**
     * @brief Get first step of returned path
     * @param start Start coordinates
     * @param end End coordinates
     * @return Pair - first step ( 'w', 'a', 's', 'd', 'n' ), and distance, returns { 'n', -1 } if no path was found
     */
    const std::pair<char, int> GetFirstStep( const std::pair<int, int> & start,
                                             const std::pair<int, int> & end,
                                             const bool & noBlock = false );

  private:
    /// Class for comparing pair<int, int>'s
    class PairIntCmp {
      public:
        /// Comparator
        const bool operator () ( const std::pair<int, int> & lhs,
                                 const std::pair<int, int> & rhs ) const {
          if ( lhs.first != rhs.first ) {
            return lhs.first < rhs.first;
          }
          return lhs.second < rhs.second;
        }
    };

    /// Game for which to find the path
    Game * m_Game;

    /// Start coordinates
    std::pair<int, int> m_Start;

    /// End coordinates
    std::pair<int, int> m_End;

    /// Use portals
    bool m_UsePortals;

    /// Ignore collisions with objects
    bool m_NoBlock;

    /// Container with all of the connections
    std::map<std::pair<int, int>, std::pair<int, int>, PairIntCmp> m_Connections;

    /// Container with visited nodes
    std::set<std::pair<int, int>, PairIntCmp> m_Visited;

    /// Bfs queue
    std::queue<std::pair<int, int> > m_Queue;

    /**
     * @brief Check if path can lead through given object
     * @param c Object's character
     * @return Boolean - yes/no
     */
    const bool ObjectIsValid( const char & c ) const;

    /**
     * @brief Check if node has already been visited
     * @param n Node to check
     * @return Boolean whether given node has been visited
     */
    const bool Visited( const std::pair<int, int> & n ) const;

    /**
     * @brief Insert node to visited list
     * @param n Node to insert
     */
    void Visit( const std::pair<int, int> & n );

    /**
     * @brief Push node to connections container
     * @param next Child node
     * @param prev Parent node
     */
    void PushToConn( const std::pair<int, int> & next,
                     const std::pair<int, int> & prev );

    /**
     * @brief Validate given node and push it to queue
     * @param parent Parent node, needed for PushToConn inside this method
     * @param n Child node, the one to validate and push to queue
     */
    void ValidatePushToQueue( const std::pair<int, int> & parent,
                              const std::pair<int, int> & n );

    /**
     * @brief Push valid child nodes from all directions to queue
     * @param n Parent node
     * @return If some child node is the end we are searching for, return true, otherwise return false
     */
    const bool PushChildrenToQueue( const std::pair<int, int> & n );

    /// Find path using the BFS algorithm from m_Start to m_End
    const bool Search();
};

#endif // BFS_PATH_FINDER_H
