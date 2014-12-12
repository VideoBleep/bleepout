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

class Brick : public GameObject, public PhysicsObject {
public:
    Brick();
  
  int value() const { return _value; }
  void setValue(int value) { _value = value; }
  
  void output(std::ostream& os) const override;
  
  const ofColor& getColor() const { return _color; }
  void setColor(const ofColor& color) { _color = color; }
    
  static const char* typeName() { return "brick"; }
private:
  int _value;
  ofColor _color;
};

#endif /* defined(__bleepout__Brick__) */
