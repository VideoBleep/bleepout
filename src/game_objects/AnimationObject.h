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
#include "Timing.h"
#include "GameObjectCollection.h"

class RoundConfig;
class RoundState;

class AnimationObject : public GameObject {
public:
  AnimationObject(float delay, float duration)
  : GameObject(GAME_OBJECT_ANIMATION)
  , _delay(delay), _duration(duration), _visible(false) { }
  
  virtual void draw(const RoundConfig& config) = 0;
  virtual void output(std::ostream& os) const override;
  
  void show() { _visible = true; }
  void hide() { _visible = false; }
  
  virtual bool visible() const override {
    return alive() && _visible;
  }
  
  DurationAction*
  createUpdaterAction(RoundState& state);
protected:
  inline float percentage() const { return _percentage; }
private:
  bool _visible;
  float _delay;
  float _duration;
  float _percentage;
  
  friend class AnimationUpdater;
};

template<>
struct GameObjectTypeTraits<AnimationObject> {
  static const GameObjectType typeId = GAME_OBJECT_ANIMATION;
  static const char typeName[];
};

#endif /* defined(__bleepout__AnimationObject__) */
