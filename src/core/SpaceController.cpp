//
//  SpaceController.cpp
//  bleepout
//
//  Created by tekt on 12/7/14.
//
//

#include "SpaceController.h"
#include "PhysicsUtil.h"

namespace {
    
  static ofVec3f getBallStartPosition(int i, int numPlayers, RoundConfig& config) {
    return ofVec3f(0, config.domeRadius() + config.domeMargin());
  }
  
}

SpaceController::SpaceController(RoundState& state, RoundConfig & config)
: _state(state), _config(config) {
}

void SpaceController::addInitialPaddles() {
  int numPlayers = _state.players().size();
  int i = 0;
  for (ofPtr<Player>& player : _state.players()) {
    addPaddle(360 * i / (numPlayers * 1.0), player.get());
    ofVec2f ballCenter = getBallStartPosition(i, numPlayers, _config);
    i++;
  }
}

void SpaceController::setup() {
  _world.setup();
  ofAddListener(_world.collisionEvent, this, &SpaceController::onCollision);
  
  for (const BallSpec& ball : _config.balls()) {
    addBall(ball);
  }
  
  for (const BrickSpec& brick : _config.allBricks()) {
    addBrick(brick);
  }
  
  for (const WallSpec& wall : _config.allWalls()) {
    addWall(wall);
  }
    
  // Create the floor exit wall
  float d = (_config.domeMargin() + _config.domeRadius()) * 5;
  addWall(WallSpec(-10, 0, ofVec3f(d, 10, d), true));
}

void SpaceController::addBrick(const BrickSpec &brickSpec) {
  Brick& brick = _state.addBrick(brickSpec);
  _world.addObject(&brick);
}

void SpaceController::addBall(const BallSpec &ballSpec) {
  Ball& ball = _state.addBall(ballSpec);
  _world.addObject(&ball);
}

void SpaceController::addPaddle(float heading, Player* player) {
    Paddle& paddle = _state.addPaddle(player);
    player->setPaddle(&paddle);
    paddle.setPositionCylindrical(_config.domeRadius() + _config.domeMargin(), heading, _config.domeMargin());
    paddle.setSize(_config.paddleSize());

    _world.addObject(&paddle);
}

void SpaceController::addWall(const WallSpec &wallSpec) {
  Wall& wall = _state.addWall(wallSpec);
  _world.addObject(&wall);
}

void SpaceController::setUpModifier(Modifier &modifier,
                                    Brick &spawnerBrick) {
  modifier.setup(_config, spawnerBrick);
  _world.addObject(&modifier);
}

void SpaceController::removeModifier(Modifier &modifier) {
  _world.removeObject(&modifier);
}

void SpaceController::update() {
    _world.update();
}

void SpaceController::onCollision(CollisionArgs &cdata) {
    if (cdata.a->type() == GAME_OBJECT_MODIFIER) {
        modifierHitObject(static_cast<Modifier*>(cdata.a), cdata.b);
    } else if (cdata.b->type() == GAME_OBJECT_MODIFIER) {
        modifierHitObject(static_cast<Modifier*>(cdata.b), cdata.a);
    } else if (cdata.a->type() == GAME_OBJECT_BALL) {
        ballHitObject(static_cast<Ball*>(cdata.a), cdata.b, cdata.normal);
    } else if (cdata.b->type() == GAME_OBJECT_BALL) {
        ballHitObject(static_cast<Ball*>(cdata.b), cdata.a, -cdata.normal);
    }
}

void SpaceController::ballHitObject(Ball *ball, GameObject *obj, ofVec3f normal) {
  switch (obj->type()) {
    case GAME_OBJECT_BRICK:
      notifyBallHitBrick(ball, static_cast<Brick*>(obj));
      ball->bounce(normal);
      break;
    case GAME_OBJECT_PADDLE:
      notifyBallHitPaddle(ball, static_cast<Paddle*>(obj));
      ball->bounce(normal);
      break;
    case GAME_OBJECT_BALL:
      notifyBallHitBall(ball, static_cast<Ball*>(obj));
      break;
    case GAME_OBJECT_WALL:
      notifyBallHitWall(ball, static_cast<Wall*>(obj));
      ball->bounce(normal);
      break;
    default:
      break;
  }
}

void SpaceController::modifierHitObject(Modifier *modifier, GameObject *obj) {
  switch (obj->type()) {
    case GAME_OBJECT_PADDLE:
      notifyModifierHitPaddle(modifier, static_cast<Paddle*>(obj));
      break;
    default:
      break;
  }
}

