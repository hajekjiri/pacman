/// Coin class
/**
 * @file Coin.h
 * Represents a coin (dot).
 */

#ifndef COIN_H
#define COIN_H

#include "SolidGameObject.h"

class Coin : public SolidGameObject {
  public:
    Coin( const std::pair<int, int> & coords );

    ~Coin();

    void Interact( Ghost & o ) override;

    void Interact( Pacman & o ) override;

  private:
    int m_Score;
};

#endif // COIN_H
