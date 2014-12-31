//
//  SpaceController.cpp
//  bleepout
//
//  Created by tekt on 12/7/14.
//
//

#include "SpaceController.h"
#include "PhysicsUtil.h"
#include "BleepoutParameters.h"
#include "PhysicsUtil.h"

namespace {
  
  static ofVec3f getBallStartPosition(int i, int numPlayers,
                                      const RoundConfig& config) {
    const auto& appParams = BleepoutParameters::get();
    return ofVec3f(0, appParams.domeRadius + appParams.domeMargin);
  }
  
}

SpaceController::SpaceController(RoundState& state)
: RoundComponent(state)
, _lastUpdateTime(-1)
, _lastBallSpeedModifier(-1)
, EventSource() {
}

void SpaceController::addInitialPaddles() {
  int numPlayers = _state.players().size();
  int i = 0;
  for (ofPtr<Player>& player : _state.players()) {
    addPaddle(360 * i / (numPlayers * 1.0), player.get());
    ofVec2f ballCenter = getBallStartPosition(i, numPlayers, _state.config());
    i++;
  }
}

void SpaceController::setup() {
  _world.setup();
  ofAddListener(_world.collisionEvent, this, &SpaceController::onCollision);
  updateBallSpeeds();
}

void SpaceController::resetState() {
  removeObjects(_state.bricks());
  removeObjects(_state.balls());
  removeObjects(_state.walls());
  removeObjects(_state.modifiers());
}

void SpaceController::updateBallSpeeds() {
  for (auto& ball : _state.balls()) {
    if (ball && ball->alive()) {
      ball->updateSpeed();
    }
  }
}

void SpaceController::loadBricksAndWalls() {
  for (const BrickSpec& brick : _state.config().allBricks()) {
    addBrick(brick);
  }
  
  for (const WallSpec& wall : _state.config().allWalls()) {
    addWall(wall);
  }
}

void SpaceController::addInitialBalls() {
  for (ofPtr<Player>& player : _state.players()) {
    BallSpec ballSpec;
    auto paddlePos = player->paddle()->getPosition();
    cartesianToSpherical(paddlePos, &ballSpec.elevation, &ballSpec.heading);
    ballSpec.elevation = 30;
    addBall(ballSpec);
  }
  for (const BallSpec& ball : _state.config().balls()) {
    addBall(ball);
  }
}

void SpaceController::addBrick(const BrickSpec &brickSpec) {
  Brick& brick = _state.addBrick(brickSpec);
  _world.addObject(&brick);
}

Ball& SpaceController::addBall(const BallSpec &ballSpec) {
  Ball& ball = _state.addBall(ballSpec);
  _world.addObject(&ball);
  return ball;
}

void SpaceController::addPaddle(float heading, Player* player) {
  const auto& appParams = BleepoutParameters::get();
  Paddle& paddle = _state.addPaddle(player);
  player->setPaddle(&paddle);
  paddle.setPositionCylindrical(appParams.domeRadius + appParams.domeMargin, heading, appParams.domeMargin);
  
  _world.addObject(&paddle);
}

void SpaceController::addWall(const WallSpec &wallSpec) {
  Wall& wall = _state.addWall(wallSpec);
  _world.addObject(&wall);
}

void SpaceController::setUpModifier(Modifier &modifier,
                                    Brick &spawnerBrick) {
  modifier.setup(_state.config(), spawnerBrick);
  _world.addObject(&modifier);
}

void SpaceController::removeObject(PhysicsObject &object) {
  _world.removeObject(&object);
}

void SpaceController::update() {
  if (_lastUpdateTime < 0) {
    _lastUpdateTime = _state.time - (1 / ofGetFrameRate());
  }
  _world.update(_state.time - _lastUpdateTime);
  _lastUpdateTime = _state.time;
  float ballSpeed = BleepoutParameters::get().ballSpeedMultiplier;
  if (ballSpeed != _lastBallSpeedModifier) {
    updateBallSpeeds();
  }
}

float paddleTrueHitFactor(const ofVec3f& paddlePos, const ofVec3f& paddleSize) {
  ofVec3f scaledPos = paddlePos / (0.5 * paddleSize);
  if (scaledPos.y == 1.0) {
    // paddle hit on top face
    float factor = 1 - abs(scaledPos.z);
    return factor * factor * 0.9;
  } else {
    // all other faces
    return 0;
  }
}

void SpaceController::onCollision(CollisionArgs &cdata) {
  auto& appParams = BleepoutParameters::get();
  if (cdata.a->type() == GAME_OBJECT_BALL) {
    Ball& ball = static_cast<Ball&>(*cdata.a);
    if (cdata.b->type() == GAME_OBJECT_PADDLE) {
      auto paddle = static_cast<Paddle&>(*cdata.b);
      ball.bounce(cdata.normalOnA, paddleTrueHitFactor(cdata.pointOnB, paddle.getSize()));
    } else if (cdata.b->type() == GAME_OBJECT_BRICK && !ball.isSupercharged()) {
      ball.bounce(cdata.normalOnA);
    } else if (cdata.b->type() == GAME_OBJECT_WALL) {
      ball.bounce(cdata.normalOnA);
    }
  } else if (cdata.b->type() == GAME_OBJECT_BALL) {
    Ball& ball = static_cast<Ball&>(*cdata.b);
    if (cdata.a->type() == GAME_OBJECT_PADDLE) {
      auto paddle = static_cast<Paddle&>(*cdata.a);
      ball.bounce(-cdata.normalOnA, paddleTrueHitFactor(cdata.pointOnA, paddle.getSize()));
    } else if (cdata.a->type() == GAME_OBJECT_BRICK && !ball.isSupercharged()) {
      ball.bounce(-cdata.normalOnA);
    } else if (cdata.a->type() == GAME_OBJECT_WALL) {
      ball.bounce(-cdata.normalOnA);
    }
  }
  notifyCollision(cdata.a, cdata.b);
}

void SpaceController::notifyCollision(GameObject *a, GameObject *b) {
  CollisionEventArgs e(a, b);
  ofNotifyEvent(collisionEvent, e);
  logEvent("Collision", e);
}
