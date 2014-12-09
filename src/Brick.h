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
  
  int value() const { return _value; }
  void setValue(int value) { _value = value; }
  
  void output(std::ostream& os) const override;
  
  static const char* typeName() { return "brick"; }
private:
  int _value;
};

#endif /* defined(__bleepout__Brick__) */
