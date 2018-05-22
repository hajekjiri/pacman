/// Blank class
/**
 * @file Blank.h
 * Represents an empty field.
 */

#ifndef BLANK_H
#define BLANK_H

#include "SolidGameObject.h"

class Map;

class Blank : public SolidGameObject {
  public:
    Blank( const std::pair<int, int> & coords );

    ~Blank();

    void Interact( Ghost & o, Map & map ) override;

    void Interact( Pacman & o, Map & map ) override;

  private:
    // TODO
};

#endif // BLANK_H
