//
//  Timing.cpp
//  bleepout
//
//  Created by tekt on 12/14/14.
//
//

#include "Timing.h"
#include <ofMain.h>

TimedActionArgs TimedActionArgs::now() {
  return TimedActionArgs(ofGetElapsedTimef(), ofGetFrameRate());
}

template<>
void ValuePulser<ofVec3f>::updateRate() {
  _rate.x = ofRandom(_minRate.x, _maxRate.x);
  _rate.y = ofRandom(_minRate.y, _maxRate.y);
  _rate.z = ofRandom(_minRate.z, _maxRate.z);
}

bool OnceAction::update(TimedActionArgs args) {
  if (_called)
    return true;
  if (args.time < _triggerTime)
    return false;
  this->call(args);
  _called = true;
  return true;
}

class FunctorOnceAction : public OnceAction {
public:
  FunctorOnceAction(float triggerTime, ofPtr<TimedFunc> fn)
  : OnceAction(triggerTime), _function(fn) { }
  
  virtual void call(TimedActionArgs args) override {
    (*_function)(args);
  }
private:
  ofPtr<TimedFunc> _function;
};

TimedAction*
OnceAction::newOnceAction(float triggerTime,
                           ofPtr<TimedFunc> fn) {
  return new FunctorOnceAction(triggerTime, fn);
}

bool DurationAction::update(TimedActionArgs args) {
  if (_ended)
    return true;
  if (!_started) {
    if (args.time >= _startTime)
      _started = true;
    else
      return false;
  } else {
    if (args.time >= _endTime) {
      _ended = true;
      return true;
    }
  }
  // note that since args isn't passed in by reference, setting
  // percentage doesn't affect the any other actions being called
  args.percentage = ofMap(args.time, _startTime, _endTime, 0, 1);
  call(args);
  return false;
}

class FunctorDurationAction : public DurationAction {
public:
  FunctorDurationAction(float start, float end, ofPtr<TimedFunc> fn)
  : DurationAction(start, end), _function(fn) { }
  
  virtual void call(TimedActionArgs args) override {
    (*_function)(args);
  }
private:
  ofPtr<TimedFunc> _function;
};

DurationAction*
DurationAction::newDurationAction(float start, float end, ofPtr<TimedFunc> fn) {
  return new FunctorDurationAction(start, end, fn);
}

bool TimedActionSet::done() const {
  for (const ofPtr<TimedAction>& action : _actions) {
    if (action && !action->done())
      return false;
  }
  return true;
}

bool TimedActionSet::update(TimedActionArgs args) {
  bool allDone = true;
  for (auto i = _actions.begin();
       i != _actions.end(); ) {
    ofPtr<TimedAction>& action = *i;
    bool done = false;
    if (!action) {
      done = true;
    } else {
      action->update(args);
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
