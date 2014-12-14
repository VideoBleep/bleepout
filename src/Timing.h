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

template<typename Arg, typename Result>
struct Timed {
  
  class Action {
  public:
    virtual Result call(Arg arg) = 0;
    
    operator bool() const { return called(); }
    
    virtual bool called() const = 0;
    
    virtual bool update(float time, Arg arg, Result* result) = 0;
  };
  
  class SimpleAction : public Action {
  protected:
    SimpleAction(float triggerTime)
    : _triggerTime(triggerTime), _called(false) { }
    
    virtual bool called() const override {
      return _called;
    }
    
    virtual bool update(float time, Arg arg, Result* result) override  {
      if (_called || time < _triggerTime)
        return false;
      *result = this->call(arg);
      _called = true;
      return true;
    }
  protected:
    bool _called;
    float _triggerTime;
  };
  
  class FunctorAction : public SimpleAction {
  public:
    typedef UnaryFunction<Arg, Result> Func;
    
    FunctorAction(float triggerTime, ofPtr<Func> f)
    : SimpleAction(triggerTime), _function(f) { }
    
    virtual Result call(Arg arg) override { return (*_function)(arg); }
  private:
    ofPtr<Func> _function;
  };
  
  class ActionSpan : public Action {
  public:
    typedef BinaryFunction<Arg, float, Result> Func;
    
    ActionSpan(float start, float end, ofPtr<Func> fn)
    :_startTime(start), _endTime(end), _started(false), _ended(false),
    _function(fn) { }
    
    Result call(float position, Arg arg) { return (*_function)(arg); }
    
    bool started() const { return _started; }
    virtual bool called() const override { return _ended; }
    
    virtual bool update(float time, Arg arg, Result* result) override {
      if (_ended)
        return false;
      if (!_started && time >= _startTime)
        _started = true;
      if (_started && time >= _endTime) {
        _ended = true;
        return true;
      }
      float position = ofMap(time, _startTime, _endTime, 0, 1);
      *result = call(position, arg);
      return true;
    }
    
  private:
    float _startTime;
    float _endTime;
    bool _started;
    bool _ended;
    const ofPtr<Func> _function;
  };
  
  class ActionSet : public Action {
  public:
    ActionSet& add(ofPtr<Action> action) {
      _actions.push_back(action);
      return *this;
    }
    ActionSet& operator+=(ofPtr<Action> action) {
      return add(action);
    }
    
    bool update(float time, Arg arg, Result* result) {
      if (_actions.empty())
        return true;
      bool allDead = true;
      for (auto i = _actions.begin();
           i != _actions.end();
           i++) {
        ofPtr<Action>& action = *i;
        if (action.update(time, arg, result)) {
          i = _actions.erase(i);
        } else {
          allDead = false;
        }
      }
      return allDead;
    }
    
  private:
    std::list<ofPtr<Action> > _actions;
  };
  
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

template<>
void ValuePulser<ofVec3f>::updateRate() {
  _rate.x = ofRandom(_minRate.x, _maxRate.x);
  _rate.y = ofRandom(_minRate.y, _maxRate.y);
  _rate.z = ofRandom(_minRate.z, _maxRate.z);
}

typedef ValuePulser<ofVec3f> SpinPulser;

#endif
