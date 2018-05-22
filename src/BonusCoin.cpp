/// Implementation of BonusCoin
/**
 * @file BonusCoin.cpp
 */

#include "BonusCoin.h"

BonusCoin::BonusCoin( const std::pair<int, int> & coords )
                    : SolidGameObject( '*', coords ) {
  // do nothing
}

BonusCoin::~BonusCoin() {
  // TODO
}

void BonusCoin::Interact( Ghost & o ) {
  // TODO
}

void BonusCoin::Interact( Pacman & o ) {
  // TODO
}
