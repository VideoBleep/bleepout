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
      _ringSet._color = _color;
      _ringSet.setLineWidth(_lineWidth);
      return Nothing();
    }
  private:
    RingSet& _ringSet;
    ofColor _color;
    float _lineWidth;
  };
  class RingFade : public TimedFunc {
  public:
    RingFade(RingSet& ringSet, ofColor startColor, ofColor endColor)
    : _ringSet(ringSet)
    , _startColor(startColor)
    , _endColor(endColor) { }
    virtual void operator()(TimedActionArgs args) override {
      ofColor color = _startColor.getLerped(_endColor, args.percentage);
      _ringSet._color = color;
    }
  private:
    RingSet& _ringSet;
    ofColor _startColor;
    ofColor _endColor;
  };
  class RingFadeAction : public ValueRampAction<ofColor> {
  public:
    RingFadeAction(RingSet& ringSet, float start, float end,
                   const ofColor& startVal, const ofColor& endVal)
    :ValueRampAction<ofColor>(start, end, startVal, endVal)
    , _ringSet(ringSet) { }
    
  protected:
    virtual void applyValue(const ofColor& value) override {
      _ringSet._color = value;
    }
  private:
    RingSet& _ringSet;
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
  TimedFunc* newFade(ofColor startColor, ofColor endColor) {
    return new RingFade(*this, startColor, endColor);
  }
  DurationAction* newFadeAction(float start, float end,
                                const ofColor& startVal, const ofColor& endVal) {
    return new RingFadeAction(*this, start, end, startVal, endVal);
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
  TimedActionSet _actions;
public:
  RendererExtrasImpl()
  : _actions(true) {
    _ringSet1.setup(SpinPulser(ofVec3f(0), ofVec3f(0.02), 5.0f, ofVec3f(0)),
                    SpinPulser(ofVec3f(0), ofVec3f(0.03), 10.0f, ofVec3f(0)),
                    ofVec3f(20), 30, 1.95, 0.4, ofColor(0, 0, 127, 63));
    _ringSet2.setup(SpinPulser(ofVec3f(0), ofVec3f(0.01), 5.0f, ofVec3f(0)),
                    SpinPulser(ofVec3f(0), ofVec3f(0.04), 40.0f, ofVec3f(0)),
                    ofVec3f(60), 10, 2.3, 0.4, ofColor(255, 127, 0, 63));
    _ringSet3.setup(SpinPulser(ofVec3f(0), ofVec3f(0.01), 5.0f, ofVec3f(0)),
                    SpinPulser(ofVec3f(0), ofVec3f(0.04), 10.0f, ofVec3f(0)),
                    ofVec3f(60), 150, 2, 0.2, ofColor(0, 127, 127, 63));
  }
  void update() {
    _actions.update(TimedActionArgs::now());
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
      _actions.add(ofPtr<TimedAction>(OnceAction::newOnceAction(ofGetElapsedTimef() + 5.0f, fn)));
    } else if (key == 'x') {
      ofColor startColor(255, 0, 0, 63);
      ofColor endColor(0, 0, 255, 63);
//      ofPtr<TimedFunc> fn(_ringSet2.newFade(startColor, endColor));
//      _actions.add(ofPtr<TimedAction>(DurationAction::newDurationAction(ofGetElapsedTimef() + 2.0f, ofGetElapsedTimef() + 8.0f, fn)));
      _actions.add(ofPtr<TimedAction>(_ringSet2.newFadeAction(ofGetElapsedTimef() + 2.0f, ofGetElapsedTimef() + 8.0f, startColor, endColor)));
    } else if (key == 'v') {
      ofLogNotice() << "queued actions: " << _actions.size();
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
