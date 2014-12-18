//
//  Brick.h
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#ifndef __bleepout__Brick__
#define __bleepout__Brick__

#include "GameObject.h"
#include "PhysicsObject.h"
#include <ofMain.h>

class RoundConfig;
struct BrickSpec;

class Brick : public GameObject, public PhysicsObject {
public:
  Brick(const RoundConfig* config = NULL, const BrickSpec* spec = NULL);
  
  int value() const { return _value; }
  void setValue(int value) { _value = value; }
  
  int lives() const { return _lives; }
  void setLives(int lives) {
    _lives = lives;
    if (_lives <= 0) {
      _lives = 0;
      kill();
    }
  }
  void adjustLives(int amount) {
    setLives(_lives + amount);
  }
  int maxLives() const { return _maxLives; }
  
  void output(std::ostream& os) const override;
  
  static const char* typeName() { return "brick"; }
private:
  int _value;
  int _lives;
  int _maxLives;
};

#endif /* defined(__bleepout__Brick__) */
