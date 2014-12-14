//
//  Common.h
//  bleepout
//
//  Created by tekt on 12/3/14.
//
//

#ifndef bleepout_Common_h
#define bleepout_Common_h

#include <ofMain.h>
#include <functional>
#include <list>

#ifdef TARGET_OSX
#define ENABLE_SYPHON
#endif

class ValueSpecifier {
public:
  enum Mode {
    CONSTANT,
    RANGE,
    RANDOM
  };
  static ValueSpecifier createConstant(float value) {
    return ValueSpecifier(CONSTANT, value, value);
  }
  static ValueSpecifier createRange(float minVal, float maxVal) {
    return ValueSpecifier(RANGE, minVal, maxVal);
  }
  static ValueSpecifier createRandom(float minVal, float maxVal) {
    return ValueSpecifier(RANDOM, minVal, maxVal);
  }
  
  ValueSpecifier(const ValueSpecifier& other)
  :_mode(other._mode), _minVal(other._minVal), _maxVal(other._maxVal) { }
  
  ValueSpecifier(Mode mode, float minVal, float maxVal)
  : _mode(mode), _minVal(minVal), _maxVal(maxVal) {}
  
  float getValue(int index, int count) {
    switch (_mode) {
      case RANGE:
        return ofMap((float)index, 0, count - 1.0f, _minVal, _maxVal);
      case RANDOM:
        return ofRandom(_minVal, _maxVal);
      case CONSTANT:
      default:
      return _minVal;
    }
  }
  
  float minValue() const { return _minVal; }
  float maxValue() const { return _maxVal; }

  bool isConstant() const { return _mode == CONSTANT; }
  bool isRandom() const { return _mode == RANDOM; }
  bool isRange() const { return _mode == RANGE; }

private:
  
  Mode _mode;
  float _minVal;
  float _maxVal;
};

template<typename Arg, typename Result>
struct Delayed {
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
    
    virtual bool update(float time, Arg arg, Result* result) override  {
      if (_called || time < Action::triggerTime())
        return false;
      *result = call(arg);
      _called = true;
      return true;
    }
  protected:
    bool _called;
    float _triggerTime;
  };
  
  class FunctorAction : public SimpleAction {
  public:
    typedef std::unary_function<Arg, Result> Func;
    
    FunctorAction(float triggerTime, ofPtr<Func> f)
    : SimpleAction(triggerTime), _function(f) { }
    
    virtual Result call(Arg arg) override { return _function(arg); }
  private:
    ofPtr<Func> _function;
  };
  
  class ActionSpan : public Action {
  public:
    typedef std::binary_function<Arg, float, Result> Func;
    
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

#endif
