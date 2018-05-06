/// Pacman class
/**
 * @file Pacman.h
 * Represents Pacman
 */

#ifndef PACMAN_H
#define PACMAN_H

#include "MovingGameObject.h"

class Pacman : public MovingGameObject {
  public:
    Pacman();

    ~Pacman();

    void Interact( const MovingGameObject & o );

    const bool IsAlive() const;

  private:
    bool m_Alive;
};

#endif // PACMAN_H
