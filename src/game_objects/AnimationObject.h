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

class RoundConfig;

class AnimationObject : public GameObject {
public:
  AnimationObject() : GameObject(GAME_OBJECT_ANIMATION) {}
  
  virtual void update(float time) = 0;
  virtual void draw(const RoundConfig& config) = 0;
private:
};

template<>
struct GameObjectTypeTraits<AnimationObject> {
  static const GameObjectType typeId = GAME_OBJECT_ANIMATION;
  static const char typeName[];
};

#endif /* defined(__bleepout__AnimationObject__) */
