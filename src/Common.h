//
//  Common.h
//  bleepout
//
//  Created by tekt on 12/3/14.
//
//

#ifndef bleepout_Common_h
#define bleepout_Common_h

#include <ofMath.h>

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

#endif
