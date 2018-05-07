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
    Coin();

    ~Coin();

    void Interact( MovingGameObject & o ) override;

    void Interact( Ghost & o ) override;

    void Interact( Pacman & o ) override;

  private:
    int m_Score;
};

#endif // COIN_H
