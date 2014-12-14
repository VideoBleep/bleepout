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
    for (int i = 0; i < numPlayers; i++) {
        ofPtr<Player> player = _state.players()[i];
        addPaddle(360 * i / (numPlayers * 1.0), player.get());
        ofVec2f ballCenter = getBallStartPosition(i, numPlayers, _config);
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
}

void SpaceController::addBrick(const BrickSpec &brickSpec) {
  ofPtr<Brick> brick(new Brick);
  brick->setPositionSpherical(_config.domeRadius() + _config.domeMargin(), brickSpec.elevation, brickSpec.heading);
  brick->setSize(_config.brickSize());
  brick->setColor(brickSpec.color);
  
  _world.addObject(brick.get());
  _state.bricks().push_back(brick);
}

void SpaceController::addBall(const BallSpec &ballSpec) {
  ofPtr<Ball> ball(new Ball);
  ball->setSize(ofVec3f(_config.ballRadius(), _config.ballRadius(), _config.ballRadius()));
  auto t = new OrbitalTrajectory();
  t->setRadius(_config.domeRadius() + _config.domeMargin());
  t->setSpeed(0.03);
  t->initWithTwoPoints(ballSpec.elevation, ballSpec.heading, -45, ballSpec.heading + ofRandom(-45, 45));
  ball->setTrajectory(t);
  
  _world.addObject(ball.get());
  _state.balls().push_back(ball);
}

void SpaceController::addPaddle(float heading, Player* player) {
    ofPtr<Paddle> paddle(new Paddle(player));
    player->setPaddle(paddle.get());
    paddle->setPositionCylindrical(_config.domeRadius() + _config.domeMargin(), heading, _config.domeMargin());
    paddle->setSize(_config.paddleSize());

    _world.addObject(paddle.get());
    _state.paddles().push_back(paddle);
}

void SpaceController::addWall(const WallSpec &wallSpec) {
  ofPtr<Wall> wall(new Wall());
  wall->setPositionSpherical(_config.domeRadius() + _config.domeMargin(), wallSpec.elevation, wallSpec.heading);
  wall->setSize(wallSpec.size);
  _world.addObject(wall.get());
  _state.walls().push_back(wall);
}

void SpaceController::update() {
    _world.update();
}

void SpaceController::onCollision(CollisionArgs &cdata) {
    if (cdata.a->type() == GAME_OBJECT_BALL) {
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

