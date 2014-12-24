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
#include "PhysicsUtil.h"

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
  explicit RingSet(const RingSetSpec& spec) {
    _spinPulser = createValuePulser(spec.spin);
    _spreadPulser = createValuePulser(spec.spread);
    _spreadOffset = spec.spreadOffset;
    _count = spec.count;
    _radiusScale = spec.radiusScale;
    _lineWidth = spec.lineWidth;
    _color = spec.color;
  }
  
  virtual void draw(RoundState& state) override {
    float radius = state.config().domeRadius() * _radiusScale;
    ofPushMatrix();
    ofPushStyle();
    
    ofNoFill();
    ofSetLineWidth(_lineWidth);
    ofSetColor(_color);
    
    _spinPulser.update(state.time);
    _spreadPulser.update(state.time);
    
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

class SphereRingSet : public Drawable {
public:
  virtual void draw(RoundState& state) override {
    float radius = state.config().domeRadius() * _radiusScale;
    ofPushMatrix();
    ofPushStyle();
    ofEnableAlphaBlending();
    ofFill();
    ofSetColor(_color);
    
    for (int i = 0; i < _count; i++) {
      float heading = ofMap((float)i, 0, _count - 1, 0, 360);
      heading += _headingPulser.update(state.time);
      ofVec3f pos = cylindricalToCartesian(radius, heading, 1.0f);
      ofDrawSphere(pos, _sphereRadius);
    }
    
    //...
    ofPopStyle();
    ofPopMatrix();
  }
//private:
  int _count;
  float _radiusScale;
  float _sphereRadius;
  ValuePulser<float> _headingPulser;
  ofColor _color;
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
//    ofPtr<SphereRingSet> spheres(new SphereRingSet());
//    spheres->_count = 30;
//    spheres->_radiusScale = 1.5;
//    spheres->_sphereRadius = 6.0f;
//    spheres->_headingPulser.setup(0, 0.02f, 5.0f, 0);
//    spheres->_color.set(0, 255, 0, 63);
//    _drawables.push_back(spheres);
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
