//
//  RendererExtras.cpp
//  bleepout
//
//  Created by tekt on 12/13/14.
//
//

#include "RendererExtras.h"
#include <ofMain.h>

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

class SpinPulser {
public:
  SpinPulser() {}
  SpinPulser(ofVec3f minRate, ofVec3f maxRate, float changeInterval, ofVec3f startValue = ofVec3f()) {
    setup(minRate, maxRate, changeInterval, startValue);
  }
  
  void setup(ofVec3f minRate, ofVec3f maxRate, float changeInterval, ofVec3f startValue = ofVec3f()) {
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
  
  const ofVec3f& value() const { return _value; }
  
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
  
  void setValue(ofVec3f value) {
    _value = value;
  }
  
private:
  void updateRate() {
    _rate.x = ofRandom(_minRate.x, _maxRate.x);
    _rate.y = ofRandom(_minRate.y, _maxRate.y);
    _rate.z = ofRandom(_minRate.z, _maxRate.z);
  }
  
private:
  Pulser _changePulser;
  ofVec3f _value;
  ofVec3f _rate;
  ofVec3f _minRate;
  ofVec3f _maxRate;
};

// there's definitely a better way to do this...
static void rotate3d(ofVec3f rotations) {
  ofRotateX(rotations.x);
  ofRotateY(rotations.y);
  ofRotateZ(rotations.z);
}

class RingSet {
public:
  RingSet() { }
  
  void setup(SpinPulser spinPulser, SpinPulser spreadPulser, ofVec3f spreadOffset, int count, float radiusScale, float lineWidth, ofColor color) {
    _spinPulser = spinPulser;
    _spreadPulser = spreadPulser;
    _spreadOffset = spreadOffset;
    _count = count;
    _radiusScale = radiusScale;
    _lineWidth = lineWidth;
    _color = color;
  }
  
  void draw(RoundConfig config) {
    float radius = config.domeRadius() * _radiusScale;
    float totalElapsed = ofGetElapsedTimef();
    float rate = ofGetFrameRate();
    ofPushMatrix();
    ofPushStyle();
    
    ofNoFill();
    ofSetLineWidth(_lineWidth);
    ofSetColor(_color);
    
    _spinPulser.update(totalElapsed, rate);
    _spreadPulser.update(totalElapsed, rate);
    
    rotate3d(_spinPulser.value());
    
    ofVec3f rotationStep = (_spreadOffset + _spreadPulser.value()) / _count;
    for (int i = 0; i < _count; ++i) {
      rotate3d(rotationStep);
      ofCircle(0, 0, 0, radius);
    }
    
    ofPopStyle();
    ofPopMatrix();
  }
private:
  SpinPulser _spinPulser;
  SpinPulser _spreadPulser;
  ofVec3f _spreadOffset;
  int _count;
  float _radiusScale;
  ofColor _color;
  float _lineWidth;
};

class RendererExtrasImpl {
private:
  RingSet _ringSet1;
  RingSet _ringSet2;
public:
  RendererExtrasImpl() {
    _ringSet1.setup(SpinPulser(ofVec3f(0), ofVec3f(0.02), 5.0f),
                    SpinPulser(ofVec3f(0), ofVec3f(0.03), 10.0f),
                    ofVec3f(20), 30, 1.95, 0.4, ofColor(127, 0, 63));
    _ringSet2.setup(SpinPulser(ofVec3f(0), ofVec3f(0.01), 5.0f),
                    SpinPulser(ofVec3f(0), ofVec3f(0.04), 10.0f),
                    ofVec3f(60), 100, 2, 0.2, ofColor(0, 127, 0, 63));
  }
  void draw(RoundState state, RoundConfig config) {
    ofPushMatrix();
    ofPushStyle();
    
    float totalElapsed = ofGetElapsedTimef();
    float rate = ofGetFrameRate();
    
    _ringSet1.draw(config);
    _ringSet2.draw(config);
    
    ofPopStyle();
    ofPopMatrix();
  }
};

void RendererExtras::setup() {
  _impl.reset(new RendererExtrasImpl());
}

void RendererExtras::draw(RoundState state, RoundConfig config) {
  if (_impl)
    _impl->draw(state, config);
}
