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

class Brick {
public:
  Brick() {}
  ofxBox2dRect& rect() { return _rect; };
private:
  ofxBox2dRect _rect;
};

#endif /* defined(__bleepout__Brick__) */
