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

class Brick : public GameObject {
public:
  Brick() : GameObject(GAME_OBJECT_BRICK) {}
  
  ofxBox2dRect& rect() { return _rect; };
  const ofxBox2dRect& rect() const { return _rect; }
private:
  ofxBox2dRect _rect;
};

#endif /* defined(__bleepout__Brick__) */
