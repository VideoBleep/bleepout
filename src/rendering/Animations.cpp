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
#include "BleepoutApp.h"

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
  
  void draw() override;
  void output(std::ostream& os) const override;
private:
  GameObjectId _brickId;
  ofColor _brickColor;
  ofVec3f _brickSize;
  ofVec3f _brickPosition;
  float _brickRotation;
};

void BrickDestructionAnimation::draw() {
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
  MessageAnimation(const MessageSpec& message, ofTrueTypeFont& font,
                   const RoundConfig& config)
  : AnimationObject(message.delay, message.duration)
  , _message(message), _font(font), _config(config) { }
  
  void draw() override;
  void output(std::ostream& os) const override;
private:
  const RoundConfig& _config;
  MessageSpec _message;
  ofTrueTypeFont& _font;
};

void MessageAnimation::draw() {
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
               _config.domeRadius() + _config.domeMargin() * (1.25 + j * 0.1),
               15 - (j * 1.1),
               30 + i * 120);
    }
  }
}

void MessageAnimation::output(std::ostream &os) const {
  os << "MessageAnimation{id:" << id()
  << ", message:" << _message.text
  << "}";
}

class ModifierAnimation : public AnimationObject {
public:
  ModifierAnimation(const ModifierSpec& modifierSpec, const GameObject& target, const RoundConfig& config, bool isRemove)
  : AnimationObject(0, config.modifierFadeTime())
  , _modifierSpec(modifierSpec), _target(target)
  , _isRemove(isRemove) { }
  
  void draw() override;
  void output(std::ostream& os) const override;
private:
  const ModifierSpec& _modifierSpec;
  const GameObject& _target;
  const bool _isRemove;
};

void ModifierAnimation::draw() {
  ofPushMatrix();
  ofPushStyle();
  ofEnableAlphaBlending();
  
  if (_target.type() == GAME_OBJECT_PADDLE) {
    const Paddle& paddle = static_cast<const Paddle&>(_target);
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofVec3f size = paddle.getSize();
    ofColor color = _modifierSpec.color;
    if (_isRemove) {
      size *= ofMap(percentage(), 0, 1, 1, 1.5);
      color.a = (unsigned char)std::floor(ofMap(percentage(),
                                                0, 1, 127, 0));
    } else {
      size *= ofMap(percentage(), 0, 1, 1.5, 1);
      color.a = (unsigned char)std::floor(ofMap(percentage(),
                                                0, 1, 0, 192));
    }
    ofTranslate(paddle.getPosition());
    ofRotateY(paddle.getRotation());
    ofFill();
    ofSetColor(color);
    ofDrawBox(size.x, size.y, size.z);
    
  } else {
    //...???
  }
  
  ofPopStyle();
  ofPopMatrix();
}

void ModifierAnimation::output(std::ostream &os) const {
  os << "ModifierAnimation{id:" << id()
  << ", type: " << _modifierSpec.type
  << ", target: " << _target.id()
  << ", action: " << (_isRemove ? "removed" : "applied")
  << "}";
}

class BallSpawnedAnimation : public AnimationObject {
public:
  BallSpawnedAnimation(const Ball& ball, const RoundConfig& config)
  : AnimationObject(0, config.ballSpawnedFadeTime())
  , _ball(ball)
  , _ballRadius(config.ballRadius()) { }
  
  void draw() override;
  void output(std::ostream& os) const override;
private:
  const Ball& _ball;
  float _ballRadius;
};

void BallSpawnedAnimation::draw() {
  ofPushMatrix();
  ofPushStyle();
  ofEnableAlphaBlending();
  ofFill();
  float radius = _ballRadius;
  radius *= ofMap(percentage(), 0, 1, 1.5, 1);
  ofColor color(192, 192, 192);
  color.a = (unsigned char)std::floor(ofMap(percentage(), 0, 1, 127, 0));
  ofSetColor(color);
  ofDrawSphere(_ball.getPosition(), radius);
  ofPopStyle();
  ofPopMatrix();
}

void BallSpawnedAnimation::output(std::ostream &os) const {
  os << "BallSpawnedAnimation{id:" << id()
     << ", ball: " << _ball.id()
     << "}";
}

#ifdef RADOME
static const char messageFontName[] = "GUI/PixelSplitter-Bold.ttf";
#else
static const char messageFontName[] = "PixelSplitter-Bold.ttf";
#endif

AppAnimationManager::AppAnimationManager(BleepoutApp& app)
: _app(app), _messageFont() {
  _messageFont.loadFont(messageFontName, 50, false, false, true);
  ofAddListener(app.roundEndedEvent, this,
                &AppAnimationManager::onRoundEnded);
}

AppAnimationManager::~AppAnimationManager() {
  ofRemoveListener(_app.roundEndedEvent, this,
                   &AppAnimationManager::onRoundEnded);
}

void AppAnimationManager::onRoundEnded(RoundEndedEventArgs &e) {
  MessageSpec message = buildRoundEndMessage(e.results());
  addMessage(message);
}

MessageSpec AppAnimationManager::buildRoundEndMessage(const RoundResults &results) const {
  MessageSpec message("Round Ended!!!", ofColor(255, 0, 127));
  message.setSize(20)
    .setTiming(0, 2)
    .setTrails(2);
  return message;
}

void AppAnimationManager::addMessage(const MessageSpec &message) {
  const RoundConfig* config = _app.currentRoundConfig();
  //....
  if (!config) {
    ofLogWarning() << "Trying to add message when roundconfig isn't available!";
    //...????
  } else {
    addAnimation(new MessageAnimation(message, _messageFont, *config));
  }
}

void AppAnimationManager::addAnimation(AnimationObject *animation) {
  _app.addAnimation(ofPtr<AnimationObject>(animation));
}

void RoundAnimationManager::addAnimation(AnimationObject *animation) {
  _roundController.addAnimation(ofPtr<AnimationObject>(animation));
}

RoundAnimationManager::RoundAnimationManager(RoundController& roundController)
: _roundController(roundController)
, _messageFont(){
  _messageFont.loadFont(messageFontName, 50, false, false, true);
  ofAddListener(_roundController.ballSpawnedEvent, this, &RoundAnimationManager::onBallSpawned);
}

RoundAnimationManager::~RoundAnimationManager() {
  ofRemoveListener(_roundController.ballSpawnedEvent, this, &RoundAnimationManager::onBallSpawned);
}

void RoundAnimationManager::addMessage(const MessageSpec &message) {
  addAnimation(new MessageAnimation(message, _messageFont, _roundController.config()));
}

void RoundAnimationManager::onBrickHit(BrickHitEventArgs &e) {
  if (!e.brick()->alive()) {
    auto anim = new BrickDestructionAnimation(*e.brick(), _roundController.config());
    addAnimation(anim);
  }
}

void RoundAnimationManager::onModifierApplied(ModifierEventArgs &e) {
  auto anim = new ModifierAnimation(e.modifier()->spec(),
                                    *e.target(),
                                    _roundController.config(), false);
  addAnimation(anim);
}

void RoundAnimationManager::onModifierRemoved(ModifierRemovedEventArgs &e) {
  auto anim = new ModifierAnimation(e.modifierSpec(),
                                    *e.target(),
                                    _roundController.config(), true);
  addAnimation(anim);
}

void RoundAnimationManager::onBallSpawned(BallStateEventArgs &e) {
  auto anim = new BallSpawnedAnimation(*e.ball(), _roundController.config());
  addAnimation(anim);
}

void RoundAnimationManager::onCountdownTick(TimerEventArgs &e) {
  int time = static_cast<int>(e.remainingTime());
  addMessage(MessageSpec("Time: " + ofToString(time), ofColor(255, 0, 0))
             .setSize(10)
             .setTiming(0, 0.9));
}

void RoundAnimationManager::attachTo(LogicController &roundEvents) {
  ofAddListener(roundEvents.brickHitEvent, this,
                &RoundAnimationManager::onBrickHit);
  ofAddListener(roundEvents.modifierAppliedEvent, this,
                &RoundAnimationManager::onModifierApplied);
  ofAddListener(roundEvents.modifierRemovedEvent, this,
                &RoundAnimationManager::onModifierRemoved);
  ofAddListener(roundEvents.countdownTickEvent, this,
                &RoundAnimationManager::onCountdownTick);
}

void RoundAnimationManager::detachFrom(LogicController &roundEvents) {
  ofRemoveListener(roundEvents.brickHitEvent, this,
                   &RoundAnimationManager::onBrickHit);
  ofRemoveListener(roundEvents.modifierAppliedEvent, this,
                   &RoundAnimationManager::onModifierApplied);
  ofRemoveListener(roundEvents.modifierRemovedEvent, this,
                   &RoundAnimationManager::onModifierRemoved);
  ofRemoveListener(roundEvents.countdownTickEvent, this,
                   &RoundAnimationManager::onCountdownTick);
}
