//
//  GameState.cpp
//  bleepout
//
//  Created by tekt on 12/7/14.
//
//

#include "GameState.h"
#include "BleepoutConfig.h"
#include "BleepoutParameters.h"

RoundState::RoundState(std::list<ofPtr<Player> >& players)
: _totalBricks(0)
, _liveBricks(0)
, _totalBalls(0)
, _liveBalls(0)
, endTime(-1) {
  for (auto& player : players) {
    addPlayer(player);
  }
}

RoundState::~RoundState() {
  for (auto& player : _players) {
    if (player && player->paddle()) {
      player->setPaddle(NULL);
    }
    player.reset();
  }
  _paddles.clear();
}

void RoundState::initialize(ofPtr<RoundConfig> config) {
  _config = config;
  _balls.clear();
  _bricks.clear();
  _walls.clear();
  _modifiers.clear();
  _animations.clear();
  _totalBalls = _liveBalls = 0;
  _totalBricks = _liveBricks = 0;
  time = 0;
  endTime = BleepoutParameters::get().rules().timeLimit();
}

void RoundState::addPlayer(ofPtr<Player> player) {
  _players.push_back(player);
}

Paddle& RoundState::addPaddle(Player* player) {
  ofPtr<Paddle> paddle(new Paddle(*player, BleepoutParameters::get().appConfig().paddleSize));
  _paddles.push_back(paddle);
  return *paddle;
}

Brick& RoundState::addBrick(const BrickSpec& brickSpec) {
  ofPtr<Brick> brick(new Brick(*_config, brickSpec));
  _bricks.push_back(brick);
  if (brick->alive())
    _liveBricks++;
  _totalBricks++;
  return *brick;
}

Wall& RoundState::addWall(const WallSpec& wallSpec) {
  ofPtr<Wall> wall(new Wall(*_config, wallSpec));
  _walls.push_back(wall);
  return *wall;
}

Ball& RoundState::addBall(const BallSpec& ballSpec) {
  ofPtr<Ball> ball(new Ball(*_config, ballSpec));
  _balls.push_back(ball);
  if (ball->alive())
    _liveBalls++;
  _totalBalls++;
  return *ball;
}

void RoundState::addModifier(ofPtr<Modifier> modifier) {
  _modifiers.push_back(modifier);
}

void RoundState::addAnimation(ofPtr<AnimationObject> animation) {
  _animations.push_back(animation);
}
