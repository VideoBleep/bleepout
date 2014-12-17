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
#include <iostream>

#ifdef TARGET_OSX
#define ENABLE_SYPHON
#endif

enum GameObjectType {
  GAME_OBJECT_OTHER,
  GAME_OBJECT_BRICK,
  GAME_OBJECT_PADDLE,
  GAME_OBJECT_BALL,
  GAME_OBJECT_PLAYER,
  GAME_OBJECT_WALL,
  GAME_OBJECT_ANIMATION,
  GAME_OBJECT_MODIFIER
};
typedef int GameObjectId;

enum ModifierType {
  MODIFIER_UNKNOWN,
  MODIFIER_EXTRA_LIFE,
  MODIFIER_PADDLE_WIDTH
};

struct Nothing { };

class Outputable {
public:
  virtual void output(std::ostream& os) const = 0;
};

std::ostream& operator<<(std::ostream& os, const Outputable& obj);

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
class UnaryFunction : public std::unary_function<Arg, Result> {
public:
  virtual Result operator()(Arg) = 0;
};

template<typename Arg1, typename Arg2, typename Result>
class BinaryFunction : public std::binary_function<Arg1, Arg2, Result> {
public:
  virtual Result operator()(Arg1, Arg2) = 0;
};

template<typename Arg>
class UnaryAction {
public:
  virtual void operator()(Arg) = 0;
};

template<typename Arg1, typename Arg2>
class BinaryAction {
public:
  virtual void operator()(Arg1, Arg2) = 0;
};

template<typename T>
T getInterpolated(const T& a, const T& b, float amount);

#endif
