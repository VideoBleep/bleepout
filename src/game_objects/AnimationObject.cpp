//
//  AnimationObject.cpp
//  bleepout
//
//  Created by tekt on 12/14/14.
//
//

#include "AnimationObject.h"
#include "GameState.h"
#include <ofUtils.h>

const char GameObjectTypeTraits<AnimationObject>::typeName[] = "animation";


class AnimationUpdater : public DurationAction {
public:
  AnimationUpdater(float start, float end,
                   AnimationObject& animation,
                   GameObjectCollection<AnimationObject>& animationList)
  : DurationAction(start, end)
  , _animation(animation)
  , _animationList(animationList) { }
  
  virtual void call(RoundState& state, float percentage) override;
  
protected:
  virtual void start() override;
  virtual void end() override;
private:
  AnimationObject& _animation;
  GameObjectCollection<AnimationObject>& _animationList;
};

void AnimationUpdater::call(RoundState& state, float percentage) {
  _animation._percentage = percentage;
}

void AnimationUpdater::start() {
  DurationAction::start();
  _animation.show();
}

void AnimationUpdater::end() {
  DurationAction::end();
  _animation.hide();
  _animationList.eraseObjectById(_animation.id());
}

void AnimationObject::output(std::ostream &os) const {
  os << "Animation{}";
}

DurationAction*
AnimationObject::createUpdaterAction(GameObjectCollection<AnimationObject>& animationList) {
  float start = ofGetElapsedTimef() + _delay;
  return new AnimationUpdater(start, start + _duration,
                              *this, animationList);
}
