//
//  GameState.cpp
//  bleepout
//
//  Created by tekt on 12/7/14.
//
//

#include "GameState.h"

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
