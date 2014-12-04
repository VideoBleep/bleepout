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
RoundController::RoundController(RoundConfig config,
                                 PlayerManager& playerManager,
                                 RendererBase& renderer)
  : _config(config),
    _playerManager(playerManager),
    _renderer(renderer) {
}

namespace {

static ofVec2f getPaddleStartPosition(int i, int numPlayers, RoundConfig& config) {
  ofVec2f pos;
  //...
  throw NotImplementedException("getPaddleStartPosition");
  return pos;
}
static ofVec2f getBallStartPosition(int i, int numPlayers, RoundConfig& config) {
  ofVec2f pos;
  //...
  throw NotImplementedException("getBallStartPosition");
  return pos;
}

}

void RoundController::setup() {
  _box2d.init();
  _box2d.createGround();
  _box2d.setFPS(BleepoutConfig::getInstance().fps());
  _box2d.enableEvents();
  ofAddListener(_box2d.contactStartEvents, this, &RoundController::contactStart);
  ofAddListener(_box2d.contactEndEvents, this, &RoundController::contactEnd);
  
  int numPlayers = _playerManager.size();
  for (int i = 0; i < numPlayers; i++) {
    Player& player = _playerManager.getPlayerAtIndex(i);
    ofVec2f paddleCenter = getPaddleStartPosition(i, numPlayers, _config);
    addPaddle(paddleCenter, player);
    ofVec2f ballCenter = getBallStartPosition(i, numPlayers, _config);
    addBall(ballCenter);
  }
  //...
}

void RoundController::draw() {
  _renderer.draw(*this);
  //...
}

void RoundController::update() {
  _box2d.update();
  //...
}

void RoundController::addBrick(ofVec2f center) {
  ofRectangle rect;
  rect.setFromCenter(center, _config.brickSize().x, _config.brickSize().y);
  ofPtr<Brick> brick(new Brick);
  brick->rect().setup(_box2d.getWorld(), rect);
  brick->rect().setData(brick.get());
  _bricks.push_back(brick);
}

void RoundController::addBall(ofVec2f center) {
  ofPtr<Ball> ball(new Ball);
  ball->circle().setup(_box2d.getWorld(), center, _config.ballRadius());
  ball->circle().setPhysics(_config.ballDensity(), _config.ballBounce(), _config.ballFriction());
  ball->circle().setData(ball.get());
  _balls.push_back(ball);
}

void RoundController::addPaddle(ofVec2f center, Player &player) {
  ofPtr<Paddle> paddle(new Paddle(player));
  ofRectangle rect;
  rect.setFromCenter(center, _config.paddleSize().x, _config.paddleSize().y);
  paddle->rect().setup(_box2d.getWorld(), rect);
  paddle->rect().setPhysics(_config.paddleBounce(), _config.paddleDensity(), _config.paddleFriction());
  paddle->rect().setData(paddle.get());
  _paddles.push_back(paddle);
}
