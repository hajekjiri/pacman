/// Coin class
/**
 * @file Coin.h
 * Represents a coin (dot).
 */

#ifndef COIN_H
#define COIN_H

#include "SolidGameObject.h"

class Map;

class Coin : public SolidGameObject {
  public:
    Coin( const std::pair<int, int> & coords );

    ~Coin();

    void Interact( Ghost * o, Map & map );

    void Interact( Pacman * o, Map & map );

  private:
    int m_Score;
};

#endif // COIN_H
