//
//  RoundManager.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "Common.h"
#include "RoundManager.h"
#include "BleepoutConfig.h"
#include "RendererBase.h"

namespace {
  
  static ofVec2f getPaddleStartPosition(int i, int numPlayers, RoundConfig& config) {
    
    ofVec2f pos;
    pos.y = ofGetHeight() - (config.brickSize().y / 2);
    float halfWidth = config.brickSize().x;
    pos.x = ofMap((float)i, 0.0f, (float)numPlayers, halfWidth, ofGetWidth() - halfWidth);
    //...
//    throw NotImplementedException("getPaddleStartPosition");
    return pos;
  }
  static ofVec2f getBallStartPosition(int i, int numPlayers, RoundConfig& config) {
    ofVec2f pos;
    pos.y = ofGetHeight() / 2;
    float halfWidth = config.brickSize().x;
    pos.x = ofMap((float)i, 0.0f, (float)numPlayers, halfWidth, ofGetWidth() - halfWidth);
    //...
//    throw NotImplementedException("getBallStartPosition");
    return pos;
  }
  
}


RoundController::RoundController(RoundConfig config,
                                 PlayerManager& playerManager,
                                 RendererBase& renderer)
  : _config(config),
    _playerManager(playerManager),
    _renderer(renderer) {
}

void RoundController::setup() {
  _box2d.init();
  _box2d.createGround();
  _box2d.setFPS(BleepoutConfig::getInstance().fps());
  _box2d.enableEvents();
  ofAddListener(_box2d.contactStartEvents, this, &RoundController::contactStart);
  ofAddListener(_box2d.contactEndEvents, this, &RoundController::contactEnd);
  
  int numPlayers = _playerManager.players().size();
  for (int i = 0; i < numPlayers; i++) {
    Player& player = *(_playerManager.players()[i]);
    ofVec2f paddleCenter = getPaddleStartPosition(i, numPlayers, _config);
    addPaddle(paddleCenter, player);
    ofVec2f ballCenter = getBallStartPosition(i, numPlayers, _config);
    addBall(ballCenter);
  }
  //...
}

void RoundController::addBrick(ofVec2f center) {
  ofRectangle rect;
  rect.setFromCenter(center, _config.brickSize().x, _config.brickSize().y);
  ofPtr<Brick> brick(new Brick);
  brick->rect().setup(_box2d.getWorld(), rect);
  brick->rect().setData(&(brick->key()));
  _bricks.add(brick);
}

void RoundController::addBall(ofVec2f center) {
  ofPtr<Ball> ball(new Ball);
  ball->circle().setup(_box2d.getWorld(), center, _config.ballRadius());
  ball->circle().setPhysics(_config.ballDensity(), _config.ballBounce(), _config.ballFriction());
  ball->circle().setData(&(ball->key()));
  _balls.add(ball);
}

void RoundController::addPaddle(ofVec2f center, Player &player) {
  ofPtr<Paddle> paddle(new Paddle(player));
  ofRectangle rect;
  rect.setFromCenter(center, _config.paddleSize().x, _config.paddleSize().y);
  paddle->rect().setup(_box2d.getWorld(), rect);
  paddle->rect().setPhysics(_config.paddleBounce(), _config.paddleDensity(), _config.paddleFriction());
  paddle->rect().setData(&(paddle->key()));
  
  _paddles.add(paddle);
}

ofPtr<GameObject> RoundController::getObject(const GameObjectKey &key) {
  switch (key.type) {
    case GAME_OBJECT_BALL:
      return ofPtr<GameObject>(_balls.getById(key.id));
    case GAME_OBJECT_BRICK:
      return ofPtr<GameObject>(_bricks.getById(key.id));
    case GAME_OBJECT_PADDLE:
      return ofPtr<GameObject>(_paddles.getById(key.id));
    case GAME_OBJECT_PLAYER:
      return ofPtr<GameObject>(_playerManager.players().getById(key.id));
    default:
      return ofPtr<GameObject>();
  }
}

void RoundController::draw() {
  _renderer.draw(*this);
  //...
}

void RoundController::update() {
  _box2d.update();
  //...
}

void RoundController::contactStart(ofxBox2dContactArgs &e) {
  if (e.a == NULL || e.b == NULL)
    return;
  GameObjectKey* keyA = static_cast<GameObjectKey*>(e.a->GetBody()->GetUserData());
  GameObjectKey* keyB = static_cast<GameObjectKey*>(e.a->GetBody()->GetUserData());
  if (keyA == NULL || keyB == NULL)
    return;
  ofPtr<GameObject> objA = getObject(*keyA);
  ofPtr<GameObject> objB = getObject(*keyB);
  if (!objA || !objB)
    return;
  if (keyA->type == GAME_OBJECT_BALL) {
    ballHitObject(static_cast<Ball&>(*objA), *objB);
  } else if (keyB->type == GAME_OBJECT_BALL) {
    ballHitObject(static_cast<Ball&>(*objB), *objA);
  }
}

void RoundController::contactEnd(ofxBox2dContactArgs &e) {
  //...
}

void RoundController::ballHitObject(Ball &ball, GameObject &obj) {
  switch (obj.type()) {
    case GAME_OBJECT_BRICK:
      ballHitBrick(ball, static_cast<Brick&>(obj));
      break;
    case GAME_OBJECT_PADDLE:
      ballHitPaddle(ball, static_cast<Paddle&>(obj));
      break;
    case GAME_OBJECT_BALL:
      // ????? insanity ensues
      break;
    default:
      break;
  }
}

void RoundController::ballHitBrick(Ball &ball, Brick &brick) {
  //...
  //notifyBallHitBrick(
}

void RoundController::ballHitPaddle(Ball &ball, Paddle &paddle) {
  ball.setLastPlayer(&(paddle.player()));
}
