//
//  Animations.cpp
//  bleepout
//
//  Created by tekt on 12/14/14.
//
//

#include "Animations.h"

#include "BleepoutConfig.h"
#include <ofMain.h>
#include "math.h"


class BrickDestruction : public DurationAction {
public:
  BrickDestruction(const Brick& brick, float start, float end)
  : DurationAction(start, end)
  , _brickColor(brick.getColor())
  , _brickSize(brick.getSize())
  , _brickPosition(brick.getPosition())
  , _brickRotation(brick.getRotation()) { }
  
  virtual void call(TimedActionArgs args) override {
    ofPushMatrix();
    ofPushStyle();
    ofEnableAlphaBlending();
    
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofVec3f size = _brickSize;
    size *= ofMap(args.percentage, 0, 1, 1, 1.5);
    ofColor color = _brickColor;
    color.a = (unsigned char)std::floor(ofMap(args.percentage, 0, 1, 255, 0));
    ofTranslate(_brickPosition);
    ofRotateY(_brickRotation);
    ofFill();
    ofSetColor(color);
    ofDrawBox(size.x, size.y, size.z);
    //...
    ofPopStyle();
    ofPopMatrix();
  }
private:
  ofColor _brickColor;
  ofVec3f _brickSize;
  ofVec3f _brickPosition;
  float _brickRotation;
};

DurationAction*
newBrickDestructionAnimation(const Brick& brick, const RoundConfig &config) {
  float time = ofGetElapsedTimef();
  float duration = config.brickFadeTime();
  return new BrickDestruction(brick, time, time + duration);
}

AnimationManager::AnimationManager(const RoundConfig& config)
: _config(config)
, _animations(true) {
  
}

void AnimationManager::onBrickDestroyed(BrickDestroyedEventArgs &e) {
  _animations.add(ofPtr<TimedAction>(newBrickDestructionAnimation(*e.brick(), _config)));
}

void AnimationManager::attachTo(RoundStateEventSource &roundEvents) {
  ofAddListener(roundEvents.brickDestroyedEvent, this,
                &AnimationManager::onBrickDestroyed);
}

void AnimationManager::detachFrom(RoundStateEventSource &roundEvents) {
  ofRemoveListener(roundEvents.brickDestroyedEvent, this,
                   &AnimationManager::onBrickDestroyed);
}

void AnimationManager::draw() {
  _animations.update(TimedActionArgs::now());
}
