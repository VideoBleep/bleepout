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
  Brick() : GameObject(GAME_OBJECT_BRICK) {}
  
  void output(std::ostream& os) const override;
private:
};

#endif /* defined(__bleepout__Brick__) */
