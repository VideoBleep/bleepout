//
//  Timing.cpp
//  bleepout
//
//  Created by tekt on 12/14/14.
//
//

#include "Timing.h"
#include <ofMain.h>
#include "GameState.h"
#include "ObjectSpecs.h"

template<>
void ValuePulser<ofVec3f>::updateRate() {
  _rate.x = ofRandom(_minRate.x, _maxRate.x);
  _rate.y = ofRandom(_minRate.y, _maxRate.y);
  _rate.z = ofRandom(_minRate.z, _maxRate.z);
}

template<>
void ValuePulser<float>::updateRate() {
  _rate = ofRandom(_minRate, _maxRate);
}

template<>
ValuePulser<ofVec3f> createValuePulser(const ValuePulserSpec<ofVec3f>& spec) {
  return SpinPulser(ofVec3f(spec.minRate),
                    ofVec3f(spec.maxRate),
                    spec.changeInterval,
                    spec.startValue);
}

ValuePulser<float> createValuePulser(const ValuePulserSpec<float>& spec) {
  return ValuePulser<float>(spec.minRate, spec.maxRate,
                            spec.changeInterval, spec.startValue);
}

bool OnceAction::update(float time) {
  if (_called)
    return true;
  if (time < _triggerTime)
    return false;
  this->call(time);
  _called = true;
  return true;
}

class FunctorOnceAction : public OnceAction {
public:
  FunctorOnceAction(float triggerTime, ofPtr<TimedFunc> fn)
  : OnceAction(triggerTime), _function(fn) { }
  virtual ~FunctorOnceAction() {}
  virtual void call(float time) override {
    (*_function)(time);
  }
private:
  ofPtr<TimedFunc> _function;
};

TimedAction*
OnceAction::newOnceAction(float triggerTime,
                          ofPtr<TimedFunc> fn) {
  return new FunctorOnceAction(triggerTime, fn);
}

bool DurationAction::update(float time) {
  if (_ended)
    return true;
  if (!_started) {
    if (time >= _startTime)
      this->start();
    else
      return false;
  } else {
    if (time >= _endTime) {
      this->end();
      return true;
    }
  }
  float percentage = ofMap(time, _startTime, _endTime, 0, 1);
  call(time, percentage);
  return false;
}

void DurationAction::start() {
  _started = true;
}

void DurationAction::end() {
  _ended = true;
}

class FunctorDurationAction : public DurationAction {
public:
  FunctorDurationAction(float start, float end, ofPtr<TimedPercentageFunc> fn)
  : DurationAction(start, end), _function(fn) { }
  virtual ~FunctorDurationAction() {}
  virtual void call(float time, float percentage) override {
    (*_function)(time, percentage);
  }
private:
  ofPtr<TimedPercentageFunc> _function;
};

DurationAction*
DurationAction::newDurationAction(float start, float end, ofPtr<TimedPercentageFunc> fn) {
  return new FunctorDurationAction(start, end, fn);
}

bool TimedActionSet::done() const {
  for (const ofPtr<TimedAction>& action : _actions) {
    if (action && !action->done())
      return false;
  }
  return true;
}

bool TimedActionSet::update(float time) {
  bool allDone = true;
  for (auto i = _actions.begin();
       i != _actions.end(); ) {
    ofPtr<TimedAction>& action = *i;
    bool done = false;
    if (!action) {
      done = true;
    } else {
      action->update(time);
      if (action->done())
        done = true;
    }
    if (done && _autoRemove) {
      i = _actions.erase(i);
    } else {
      i++;
    }
    if (!done)
      allDone = false;
  }
  return allDone;
}

void TimedActionSet::clear() {
  _actions.clear();
}
