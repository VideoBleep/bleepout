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
  Pulser(float interval) : _interval(interval) { }
  
  bool update(float time) {
    if ((time - _lastPulseTime) >= _interval) {
      _lastPulseTime = time;
      return true;
    }
    return false;
  }
private:
  float _interval;
  float _lastPulseTime;
};

class SpinPulser {
public:
  SpinPulser(ofVec3f minRate, ofVec3f maxRate,
             float changeInterval, ofVec3f startValue = ofVec3f())
  : _minRate(minRate), _maxRate(maxRate), _value(startValue), _changePulser(changeInterval) {
    updateRate();
  }
  
  const ofVec3f& update(float time, float frameRate) {
    if (_changePulser.update(time)) {
      updateRate();
    }
    _value += _rate * frameRate;
    return _value;
  }
  
  const ofVec3f& value() const { return _value; }
  
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

static void drawRingSet(ofVec3f axis, int count, float radius, float startAngle, float endAngle) {
  ofPushMatrix();
  ofPushStyle();

  ofNoFill();
  ofSetLineWidth(0.4f);
  ofSetColor(127, 0, 0);
  ofRotate(startAngle, axis.x, axis.y, axis.z);
  float step = (endAngle - startAngle) / count;
  for (int i = 0 ; i < count; ++i) {
    ofRotate(step, axis.x, axis.y, axis.z);
    ofCircle(0, 0, 0, radius);
  }
  
  ofPopStyle();
  ofPopMatrix();
}

static void drawRingSet_2(ofVec3f rotations, ofVec3f rotationRanges, int count, float radius) {
  ofPushMatrix();
  ofPushStyle();
  
  ofNoFill();
  ofSetLineWidth(0.4f);
  ofSetColor(127, 0, 0);
  
  ofRotateX(rotations.x);
  ofRotateY(rotations.y);
  ofRotateZ(rotations.z);
  
  ofVec3f rotationStep = rotationRanges / count;
  for (int i = 0; i < count; ++i) {
    ofRotateX(rotationStep.x);
    ofRotateY(rotationStep.y);
    ofRotateZ(rotationStep.z);
    ofCircle(0, 0, 0, radius);
  }
  
  ofPopStyle();
  ofPopMatrix();
}

class RendererExtrasImpl {
private:
  float _lastFrameTime;
  ofVec3f _axis1;
  float _angleOffset;
  ofVec3f _rotationRates;
  ofVec3f _rotations;
  ofPtr<SpinPulser> _spinPulser;
public:
  RendererExtrasImpl() {
    _axis1.set(0, 1, 0.3);
    _lastFrameTime = ofGetElapsedTimef();
    _angleOffset = 0.0f;
    _rotationRates.set(0.1, 0.3, 0.4);
    _rotations.set(0, 0, 0);
    _spinPulser.reset(new SpinPulser(ofVec3f(0.01), ofVec3f(0.3), 200.0f));
  }
  void draw(RoundState state, RoundConfig config) {
    ofPushMatrix();
    ofPushStyle();
    
    float totalElapsed = ofGetElapsedTimef();
    float timeChange = totalElapsed - _lastFrameTime;
    _lastFrameTime = totalElapsed;

    float rate = ofGetFrameRate();
    //_axis1.rotate(rate / 20.0f, ofVec3f(0, 0.2, 1));
    
    _rotations += _rotationRates * rate;
    
    drawRingSet_2(_spinPulser->update(totalElapsed, rate), _rotationRates * rate, 30, config.domeRadius() * 1.95f);
    
    _angleOffset += rate / 20.0f;
//    drawRingSet(_axis1, 30, config.domeRadius() * 1.95f, _angleOffset, 360.0f + _angleOffset);
    
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
