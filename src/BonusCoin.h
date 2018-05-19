/// BonusCoin class
/**
 * @file BonusCoin.h
 * Represents a bonus coin / "power-up"
 */
#ifndef BONUS_COIN_H
#define BONUS_COIN_H

#include "SolidGameObject.h"

class BonusCoin : public SolidGameObject {
  public:
    BonusCoin();

    ~BonusCoin();

    void Interact( MovingGameObject & o ) override;

    void Interact( Ghost & o ) override;

    void Interact( Pacman & o ) override;

  private:
    // TODO
};

#endif // BONUS_COIN_H
