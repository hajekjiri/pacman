/// BonusCoin class
/**
 * @file BonusCoin.h
 * Represents a bonus coin / "power-up"
 */
#ifndef BONUS_COIN_H
#define BONUS_COIN_H

#include "SolidGameObject.h"

class Map;

class BonusCoin : public SolidGameObject {
  public:
    BonusCoin( const std::pair<int, int> & coords );

    ~BonusCoin();

    void Interact( Ghost & o, Map & map ) override;

    void Interact( Pacman & o, Map & map ) override;

  private:
    // TODO
};

#endif // BONUS_COIN_H
