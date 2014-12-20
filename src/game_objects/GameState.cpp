//
//  GameState.cpp
//  bleepout
//
//  Created by tekt on 12/7/14.
//
//

#include "GameState.h"

RoundState::RoundState(const RoundConfig& config,
                       std::list<ofPtr<Player> >& players)
: _config(config)
, _liveBricks(0) {
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
}

void RoundState::addPlayer(ofPtr<Player> player) {
  _players.push_back(player);
}

Paddle& RoundState::addPaddle(Player* player) {
  ofPtr<Paddle> paddle(new Paddle(player));
  _paddles.push_back(paddle);
  return *paddle;
}

Brick& RoundState::addBrick(const BrickSpec& brickSpec) {
  ofPtr<Brick> brick(new Brick(&_config, &brickSpec));
  _bricks.push_back(brick);
  if (brick->alive())
    _liveBricks++;
  return *brick;
}

Wall& RoundState::addWall(const WallSpec& wallSpec) {
  ofPtr<Wall> wall(new Wall(&_config, &wallSpec));
  _walls.push_back(wall);
  return *wall;
}

Ball& RoundState::addBall(const BallSpec& ballSpec) {
  ofPtr<Ball> ball(new Ball(&_config, &ballSpec));
  _balls.push_back(ball);
  return *ball;
}

void RoundState::addModifier(ofPtr<Modifier> modifier) {
  _modifiers.push_back(modifier);
}

void RoundState::addAnimation(ofPtr<AnimationObject> animation) {
  _animations.push_back(animation);
}
