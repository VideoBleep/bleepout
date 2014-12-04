//
//  RoundManager.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "RoundManager.h"
#include "BleepoutConfig.h"

RoundController::RoundController(RoundConfig config,
                                 PlayerManager& playerManager)
  : _config(config),
    _playerManager(playerManager) {
}

static ofVec2f getPaddleStartPosition(int i, int numPlayers, RoundConfig& config) {
  ofVec2f pos;
  //...
  return pos;
}
static ofVec2f getBallStartPosition(int i, int numPlayers, RoundConfig& config) {
  ofVec2f pos;
  //...
  return pos;
}

void RoundController::setup() {
  _box2d.init();
  _box2d.createGround();
  _box2d.setFPS(BleepoutConfig::getInstance().fps());
  
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
  //...
}

void RoundController::update() {
  //...
}

void RoundController::addBrick(ofVec2f center) {
  ofRectangle rect;
  rect.setFromCenter(center, _config.brickSize().x, _config.brickSize().y);
  ofPtr<Brick> brick(new Brick);
  brick->rect().setup(_box2d.getWorld(), rect);
  _bricks.push_back(brick);
}

void RoundController::addBall(ofVec2f center) {
  ofPtr<Ball> ball(new Ball);
  ball->circle().setup(_box2d.getWorld(), center, _config.ballRadius());
  ball->circle().setPhysics(_config.ballDensity(), _config.ballBounce(), _config.ballFriction());
  _balls.push_back(ball);
}

void RoundController::addPaddle(ofVec2f center, Player &player) {
  ofPtr<Paddle> paddle(new Paddle(player));
  ofRectangle rect;
  rect.setFromCenter(center, _config.paddleSize().x, _config.paddleSize().y);
  paddle->rect().setup(_box2d.getWorld(), rect);
  paddle->rect().setPhysics(_config.paddleBounce(), _config.paddleDensity(), _config.paddleFriction());
  _paddles.push_back(paddle);
}
