//
//  Wall.h
//  bleepout
//
//  Created by tekt on 12/7/14.
//
//

#ifndef __bleepout__Wall__
#define __bleepout__Wall__

#include "GameObject.h"
#include <ofxBox2d.h>

class Wall : public GameObject, public ofxBox2dEdge {
public:
  Wall() : GameObject(GAME_OBJECT_WALL) {}
};


#endif /* defined(__bleepout__Wall__) */
