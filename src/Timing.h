//
//  Timing.h
//  bleepout
//
//  Created by tekt on 12/14/14.
//
//

#ifndef bleepout_Timing_h
#define bleepout_Timing_h

#include "Common.h"
#include <list>

struct TimedActionArgs {
  float time;
  float fps;
  float percentage;
  
  TimedActionArgs(float t, float f)
  : time(t), fps(f), percentage(0) { }
  
  static TimedActionArgs now();
};

typedef UnaryAction<TimedActionArgs> TimedFunc;

class TimedAction {
public:
  virtual bool done() const = 0;
  virtual bool update(TimedActionArgs args) = 0;
};

class OnceAction : public TimedAction {
public:
  static TimedAction* newOnceAction(float triggerTime,
                                    ofPtr<TimedFunc> fn);

  OnceAction(float triggerTime)
  : _triggerTime(triggerTime), _called(false) { }
  
  virtual bool done() const override { return _called; }
  
  virtual void call(TimedActionArgs args) = 0;
  
  virtual bool update(TimedActionArgs args) override;
protected:
  bool _called;
  float _triggerTime;
};

class DurationAction : public TimedAction {
public:
  static DurationAction*
  newDurationAction(float start, float end, ofPtr<TimedFunc> fn);
  

  DurationAction(float start, float end)
  : _startTime(start), _endTime(end) { }
  
  virtual void call(TimedActionArgs args) = 0;
  
  bool started() const { return _started; }
  virtual bool done() const override { return _ended; }
  
  virtual bool update(TimedActionArgs args) override;
  
private:
  float _startTime;
  float _endTime;
  bool _started;
  bool _ended;
};

class TimedActionSet : public TimedAction {
public:
  TimedActionSet(bool autoRemove = true)
  : _autoRemove(autoRemove) { }
  
  void add(ofPtr<TimedAction> action) {
    _actions.push_back(action);
  }
  
  virtual bool done() const override;
  
  virtual bool update(TimedActionArgs args) override;
  
  int size() const { return _actions.size(); }
private:
  bool _autoRemove;
  std::list<ofPtr<TimedAction> > _actions;
};

/**
 If _interval has elapsed since the last call to update(),
 update() should return true to indicate that a pulse is occurring.
 */
class Pulser {
public:
  Pulser() : _interval(0) { }
  Pulser(float interval) : _interval(interval) { }
  
  bool update(float time) {
    if ((time - _lastPulseTime) >= _interval) {
      pulse(time);
      return true;
    }
    return false;
  }
  
  void setInterval(float interval) {
    _interval = interval;
  }
  
  void pulse(float time) {
    _lastPulseTime = time;
  }
private:
  float _interval;
  float _lastPulseTime;
};

template<typename T>
class ValuePulser {
public:
  ValuePulser() { }
  ValuePulser(T minRate, T maxRate, float changeInterval, T startValue) {
    setup(minRate, maxRate, changeInterval, startValue);
  }
  
  void setup(T minRate, T maxRate, float changeInterval, T startValue) {
    setRange(minRate, maxRate);
    setPulseInterval(changeInterval);
    setValue(startValue);
    updateRate();
  }
  
  const ofVec3f& update(float time, float rate) {
    if (_changePulser.update(time)) {
      updateRate();
    }
    _value += _rate * rate;
    return _value;
  }
  
  const T& value() const { return _value; }
  void setValue(T value) { _value = value; }
  
  void pulse(float time) {
    _changePulser.pulse(time);
    updateRate();
  }
  
  void setRange(ofVec3f minRate, ofVec3f maxRate) {
    _minRate = minRate;
    _maxRate = maxRate;
  }
  
  void setPulseInterval(float interval) {
    _changePulser.setInterval(interval);
  }
protected:
  void updateRate();
  
  Pulser _changePulser;
  T _value;
  T _rate;
  T _minRate;
  T _maxRate;
};

typedef ValuePulser<ofVec3f> SpinPulser;

#endif
