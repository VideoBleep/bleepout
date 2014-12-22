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
#include "RendererUtil.h"
#include "LogicController.h"
#include <ofMain.h>
#include <ofTrueTypeFont.h>
#include "Logging.h"

class BrickDestructionAnimation : public AnimationObject {
public:
  BrickDestructionAnimation(const Brick& brick,
                            const RoundConfig& config)
  : AnimationObject(0, config.brickFadeTime())
  , _brickId(brick.id())
  , _brickColor(brick.getColor())
  , _brickSize(brick.getSize())
  , _brickPosition(brick.getPosition())
  , _brickRotation(brick.getRotation()) { }
  
  virtual void draw(const RoundConfig& config) override;
  virtual void output(std::ostream& os) const override;
private:
  GameObjectId _brickId;
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

void BrickDestructionAnimation::output(std::ostream &os) const {
  os << "BrickDestructionAnimation{id:" << id()
  << ", brick:" << _brickId
  << "}";
}

class MessageAnimation : public AnimationObject {
public:
  MessageAnimation(const MessageSpec& message, ofTrueTypeFont& font)
  : AnimationObject(message.delay, message.duration)
  , _message(message), _font(font) { }
  
  virtual void draw(const RoundConfig& config) override;
  virtual void output(std::ostream& os) const override;
private:
  const MessageSpec& _message;
  ofTrueTypeFont& _font;
};

void MessageAnimation::draw(const RoundConfig &config) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < _message.trails + 1; j++) {
      ofColor color = _message.color;
      float h, s, b;
      color.getHsb(h, s, b);
      color.setHsb(h + j * 16, s, b + j * 32);
      drawText(_message.text,
               color,
               _font,
               _message.size - (j * 1.5),
               config.domeRadius() + config.domeMargin() * (1.25 + j * 0.1),
               15 - (j * 1.1),
               30 + i * 120);
    }
  }
  //...
}

void MessageAnimation::output(std::ostream &os) const {
  os << "MessageAnimation{id:" << id()
  << ", message:" << _message.text
  << "}";
}

AnimationManager::AnimationManager(RoundController& roundController)
: _roundController(roundController)
, _messageFont(){
  _messageFont.loadFont("PixelSplitter-Bold.ttf", 50, false, false, true);
}

void AnimationManager::addAnimation(AnimationObject *animation) {
  _roundController.addAnimation(ofPtr<AnimationObject>(animation));
}

void AnimationManager::addMessage(const MessageSpec &message) {
  addAnimation(new MessageAnimation(message, _messageFont));
}

void AnimationManager::onBrickDestroyed(BrickDestroyedEventArgs &e) {
  auto anim = new BrickDestructionAnimation(*e.brick(), _roundController.config());
  addAnimation(anim);
}

void AnimationManager::attachTo(LogicController &roundEvents) {
  ofAddListener(roundEvents.brickDestroyedEvent, this,
                &AnimationManager::onBrickDestroyed);
}

void AnimationManager::detachFrom(LogicController &roundEvents) {
  ofRemoveListener(roundEvents.brickDestroyedEvent, this,
                   &AnimationManager::onBrickDestroyed);
}
