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
#include "Animations.h"
#include "ObjectSpecs.h"
#include <vector>

// there's definitely a better way to do this...
static void rotate3d(ofVec3f rotations) {
  ofRotateX(rotations.x);
  ofRotateY(rotations.y);
  ofRotateZ(rotations.z);
}

class Drawable {
public:
  virtual void draw(RoundState& state) = 0;
};

class RingSet : public Drawable {
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
  explicit RingSet(const RingSetSpec& spec) {
    _spinPulser = createSpinPulser(spec.spin);
    _spreadPulser = createSpinPulser(spec.spread);
    _spreadOffset = spec.spreadOffset;
    _count = spec.count;
    _radiusScale = spec.radiusScale;
    _lineWidth = spec.lineWidth;
    _color = spec.color;
  }
  
  virtual void draw(RoundState& state) override {
    float radius = state.config().domeRadius() * _radiusScale;
    float totalElapsed = ofGetElapsedTimef();
    float rate = ofGetFrameRate();
    ofPushMatrix();
    ofPushStyle();
    
    ofNoFill();
    ofSetLineWidth(_lineWidth);
    ofSetColor(_color);
    
    _spinPulser.update(totalElapsed);
    _spreadPulser.update(totalElapsed);
    
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
  std::vector<ofPtr<Drawable> > _drawables;
public:
  RendererExtrasImpl(const RoundConfig& config) {
    for (const auto& spec : config.ringSets()) {
      ofPtr<RingSet> ringSet(new RingSet(spec));
      _drawables.push_back(ringSet);
    }
  }
  void update(RoundState& state) { }
  void draw(RoundState& state) {
    ofPushMatrix();
    ofPushStyle();
    
    for (auto& obj : _drawables) {
      obj->draw(state);
    }
    
    ofPopStyle();
    ofPopMatrix();
  }
  void keyPressed(int key) { }
};

void RendererExtras::setup(const RoundConfig& config) {
  _impl.reset(new RendererExtrasImpl(config));
}

void RendererExtras::update(RoundState& state) {
  if (_impl)
    _impl->update(state);
}

void RendererExtras::draw(RoundState& state) {
  if (_impl)
    _impl->draw(state);
}

void RendererExtras::keyPressed(int key) {
  if (_impl)
    _impl->keyPressed(key);
}
