//
//  Animations.cpp
//  bleepout
//
//  Created by tekt on 12/14/14.
//
//

#include "Animations.h"

#include "BleepoutConfig.h"
#include "AnimationObject.h"
#include "RoundManager.h"
#include <ofMain.h>

class BrickDestructionAnimation : public AnimationObject {
public:
  BrickDestructionAnimation(const Brick& brick,
                            const RoundConfig& config)
  : AnimationObject(0, config.brickFadeTime())
  , _brickColor(brick.getColor())
  , _brickSize(brick.getSize())
  , _brickPosition(brick.getPosition())
  , _brickRotation(brick.getRotation()) { }
  
  virtual void draw(const RoundConfig& config) override;
private:
  ofColor _brickColor;
  ofVec3f _brickSize;
  ofVec3f _brickPosition;
  float _brickRotation;
};

void BrickDestructionAnimation::draw(const RoundConfig &config) {
  ofPushMatrix();
  ofPushStyle();
  ofEnableAlphaBlending();
  
  ofSetRectMode(OF_RECTMODE_CENTER);
  ofVec3f size = _brickSize;
  size *= ofMap(percentage(), 0, 1, 1, 1.5);
  ofColor color = _brickColor;
  color.a = (unsigned char)std::floor(ofMap(percentage(),
                                            0, 1, 255, 0));
  ofTranslate(_brickPosition);
  ofRotateY(_brickRotation);
  ofFill();
  ofSetColor(color);
  ofDrawBox(size.x, size.y, size.z);
  //...
  ofPopStyle();
  ofPopMatrix();
}

AnimationManager::AnimationManager(RoundController& roundController)
: _roundController(roundController) { }

void AnimationManager::addAnimation(AnimationObject *animation) {
  _roundController.state().addAnimation(ofPtr<AnimationObject>(animation));
}

void AnimationManager::onBrickDestroyed(BrickDestroyedEventArgs &e) {
  auto anim = new BrickDestructionAnimation(*e.brick(), _roundController.config());
  _roundController.addAnimation(ofPtr<AnimationObject>(anim));
}

void AnimationManager::attachTo(RoundStateEventSource &roundEvents) {
  ofAddListener(roundEvents.brickDestroyedEvent, this,
                &AnimationManager::onBrickDestroyed);
}

void AnimationManager::detachFrom(RoundStateEventSource &roundEvents) {
  ofRemoveListener(roundEvents.brickDestroyedEvent, this,
                   &AnimationManager::onBrickDestroyed);
}
