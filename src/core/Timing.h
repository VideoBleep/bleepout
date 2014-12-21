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
#include <ofTypes.h>

class RoundState;

typedef UnaryAction<RoundState&> TimedFunc;
typedef BinaryAction<RoundState&, float> TimedPercentageFunc;

class TimedAction {
public:
  virtual bool done() const = 0;
  virtual bool update(RoundState& args) = 0;
};

class OnceAction : public TimedAction {
public:
  static TimedAction* newOnceAction(float triggerTime,
                                    ofPtr<TimedFunc> fn);

  OnceAction(float triggerTime)
  : _triggerTime(triggerTime), _called(false) { }
  
  virtual bool done() const override { return _called; }
  
  virtual void call(RoundState& state) = 0;
  
  virtual bool update(RoundState& state) override;
protected:
  bool _called;
  float _triggerTime;
};

class DurationAction : public TimedAction {
public:
  static DurationAction*
  newDurationAction(float start, float end, ofPtr<TimedPercentageFunc> fn);

  DurationAction(float start, float end)
  : _startTime(start), _endTime(end), _started(false), _ended(false) { }
  
  bool started() const { return _started; }
  virtual bool done() const override { return _ended; }
  
  virtual bool update(RoundState& state) override;

protected:
  virtual void call(RoundState& state, float percentage) = 0;
  virtual void start();
  virtual void end();

private:
  float _startTime;
  float _endTime;
  bool _started;
  bool _ended;
};

template<typename T>
class ValueRampAction : public DurationAction {
public:
  ValueRampAction(float start, float end,
                  const T& startVal, const T& endVal)
  : DurationAction(start, end)
  , _startVal(startVal), _endVal(endVal) { }
  
  virtual void call(RoundState& state, float percentage) override {
    T value = getInterpolated(_startVal, _endVal, percentage);
    this->applyValue(value);
  }
protected:
  virtual void applyValue(const T& value) = 0;
private:
  const T& _startVal;
  const T& _endVal;
};

class TimedActionSet : public TimedAction {
public:
  TimedActionSet(bool autoRemove = true)
  : _autoRemove(autoRemove) { }
  
  void add(ofPtr<TimedAction> action) {
    _actions.push_back(action);
  }
  
  virtual bool done() const override;
  
  virtual bool update(RoundState& state) override;
  
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
  
  const ofVec3f& update(float time) {
    if (_changePulser.update(time)) {
      updateRate();
    }
    _value += _rate;
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
