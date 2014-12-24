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
#include "Logging.h"

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
//  ofLogNotice() << "Starting animation update action: " << _animation;
  DurationAction::start();
  _animation.show();
}

void AnimationUpdater::end() {
//  ofLogNotice() << "Ending animation update action: " << _animation;
  DurationAction::end();
  _animation.hide();
  _animationList.eraseObjectById(_animation.id());
}

void AnimationObject::output(std::ostream &os) const {
  os << "Animation{}";
}

DurationAction*
AnimationObject::createUpdaterAction(RoundState& state) {
  float now = state.time;
  float start = now + _delay;
//  ofLogNotice() << "Creating updater action to start at " << start << " and end at " << (start + _duration) << ". Current time: " << now;
  return new AnimationUpdater(start, start + _duration,
                              *this, state.animations());
}
