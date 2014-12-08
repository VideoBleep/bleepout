//
//  Brick.h
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#ifndef __bleepout__Brick__
#define __bleepout__Brick__

#include <ofMain.h>
#include <ofxBox2d.h>
#include "GameObject.h"

class Brick : public GameObject, public ofxBox2dRect {
public:
  Brick() : GameObject(GAME_OBJECT_BRICK), _value(1) {}
  
  void output(std::ostream& os) const override;
  
  int value() const { return _value; }
  void setValue(int value) { _value = value; }
private:
  int _value;
};

#endif /* defined(__bleepout__Brick__) */
