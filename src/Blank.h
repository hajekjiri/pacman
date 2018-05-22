/// Blank class
/**
 * @file Blank.h
 * Represents an empty field.
 */

#ifndef BLANK_H
#define BLANK_H

#include "SolidGameObject.h"

class Blank : public SolidGameObject {
  public:
    Blank( const std::pair<int, int> & coords );

    ~Blank();

    void Interact( MovingGameObject & o ) override;

    void Interact( Ghost & o ) override;

    void Interact( Pacman & o ) override;

  private:
    // TODO
};

#endif // BLANK_H
