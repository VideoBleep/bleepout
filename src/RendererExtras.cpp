//
//  RendererExtras.cpp
//  bleepout
//
//  Created by tekt on 12/13/14.
//
//

#include "RendererExtras.h"
#include <ofMain.h>
#include "Timing.h"

// there's definitely a better way to do this...
static void rotate3d(ofVec3f rotations) {
  ofRotateX(rotations.x);
  ofRotateY(rotations.y);
  ofRotateZ(rotations.z);
}

class RingSet {
private:
  class RingChange : public TimedFunc {
  public:
    RingChange(RingSet& ringSet, ofColor color, float lineWidth)
    : _ringSet(ringSet), _color(color), _lineWidth(lineWidth) { }
    virtual void operator()(TimedActionArgs args) override {
      _ringSet.color().set(_color);
      _ringSet.setLineWidth(_lineWidth);
      return Nothing();
    }
  private:
    RingSet& _ringSet;
    ofColor _color;
    float _lineWidth;
  };
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
  
  ofColor& color() { return _color; }
  
  float lineWidth() const { return _lineWidth; }
  void setLineWidth(float lineWidth) { _lineWidth = lineWidth; }
  
  TimedFunc* newChange(ofColor color, float lineWidth) {
    return new RingChange(*this, color, lineWidth);
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
  RingSet _ringSet3;
  ofPtr<TimedAction> _action;
public:
  RendererExtrasImpl() {
    _ringSet1.setup(SpinPulser(ofVec3f(0), ofVec3f(0.02), 5.0f, ofVec3f(0)),
                    SpinPulser(ofVec3f(0), ofVec3f(0.03), 10.0f, ofVec3f(0)),
                    ofVec3f(20), 30, 1.95, 0.4, ofColor(0, 0, 127, 63));
    _ringSet2.setup(SpinPulser(ofVec3f(0), ofVec3f(0.01), 5.0f, ofVec3f(0)),
                    SpinPulser(ofVec3f(0), ofVec3f(0.04), 10.0f, ofVec3f(0)),
                    ofVec3f(60), 100, 2, 0.2, ofColor(0, 127, 0, 63));
    _ringSet2.setup(SpinPulser(ofVec3f(0), ofVec3f(0.01), 5.0f, ofVec3f(0)),
                    SpinPulser(ofVec3f(0), ofVec3f(0.04), 10.0f, ofVec3f(0)),
                    ofVec3f(60), 150, 2, 0.2, ofColor(0, 127, 127, 63));
  }
  void update() {
    if (_action) {
      if (_action->update()) {
        _action.reset();
      }
    }
  }
  void draw(RoundState state, RoundConfig config) {
    ofPushMatrix();
    ofPushStyle();
    
    float totalElapsed = ofGetElapsedTimef();
    float rate = ofGetFrameRate();
    
    _ringSet1.draw(config);
    _ringSet2.draw(config);
    _ringSet3.draw(config);
    
    ofPopStyle();
    ofPopMatrix();
  }
  void keyPressed(int key) {
    if (key == 'z') {
      _ringSet1.color().setHueAngle(_ringSet1.color().getHueAngle() + 20);
      ofColor newColor(_ringSet1.color());
      newColor.setHueAngle((newColor.getHueAngle() + 30));
      ofPtr<TimedFunc> fn(_ringSet1.newChange(newColor, _ringSet1.lineWidth() + 4));
      _action.reset(OnceAction::newOnceAction(ofGetElapsedTimef() + 5.0f, fn));
    }
  }
};

void RendererExtras::setup() {
  _impl.reset(new RendererExtrasImpl());
}

void RendererExtras::update() {
  if (_impl)
    _impl->update();
}

void RendererExtras::draw(RoundState state, RoundConfig config) {
  if (_impl)
    _impl->draw(state, config);
}

void RendererExtras::keyPressed(int key) {
  if (_impl)
    _impl->keyPressed(key);
}
