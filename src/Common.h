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
class DelayedAction {
public:
  DelayedAction(Result (*f)(Arg), float triggerTime)
  : _triggerTime(triggerTime),
  _function(std::ptr_fun(f)),
  _called(false) { }
  
  Result call(Arg arg) { return _function(arg); }
  
  operator bool() const { return _called; }
  
  bool update(float time, Arg arg, Result* result) {
    if (_called || time < _triggerTime)
      return false;
    *result = call(arg);
    _called = true;
    return true;
  }
private:
  float _triggerTime;
  bool _called;
  const std::pointer_to_unary_function<Arg, Result> _function;
};

template<typename Arg, typename Result>
class DelayedActionSpan {
public:
  DelayedActionSpan(ofPtr<std::binary_function<Arg, float, Result> > fn, float start, float end)
  :_startTime(start), _endTime(end), _started(false), _ended(false),
  _function(fn) { }
  
  Result call(float position, Arg arg) { return (*_function)(arg); }
  
  operator bool() const { return _ended; }
  
  bool started() const { return _started; }
  bool ended() const { return _ended; }
  
  bool update(float time, Arg arg, Result* result) {
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
  const ofPtr<std::binary_function<Arg, float, Result> > _function;
};

template<typename Arg, typename Result>
class DelayedActionQueue {
public:
private:
  
};

#endif
