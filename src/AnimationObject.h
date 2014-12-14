//
//  AnimationObject.h
//  bleepout
//
//  Created by tekt on 12/14/14.
//
//

#ifndef __bleepout__AnimationObject__
#define __bleepout__AnimationObject__

#include "GameObject.h"
#include "PhysicsObject.h"

class AnimationObject : public GameObject {
public:
  AnimationObject() : GameObject(GAME_OBJECT_ANIMATION) {}
  
  virtual void update(float time) = 0;
private:
};

#endif /* defined(__bleepout__AnimationObject__) */
