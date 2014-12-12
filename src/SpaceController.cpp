//
//  SpaceController.cpp
//  bleepout
//
//  Created by tekt on 12/7/14.
//
//

#include "SpaceController.h"

namespace {
    
  static ofVec3f getBallStartPosition(int i, int numPlayers, RoundConfig& config) {
    return ofVec3f(0, config.domeRadius() + config.domeMargin());
  }
  
}

SpaceController::SpaceController(RoundState& state, RoundConfig & config)
: _state(state), _config(config) {
}

void SpaceController::setup() {
  _world.setup();
  ofAddListener(_world.collisionEvent, this, &SpaceController::onCollision);
  
  int numPlayers = _state.players().size();
  for (int i = 0; i < numPlayers; i++) {
    ofPtr<Player> player = _state.players()[i];
    addPaddle(360 * i / (numPlayers * 1.0), player.get());
    ofVec2f ballCenter = getBallStartPosition(i, numPlayers, _config);
  }

  addBall(30, ofRandom(360));
  addBall(30, ofRandom(360));
  addBall(30, ofRandom(360));
  
  for (int i = 0; i < 16; i++) {
    addBrick(45, ofRandom(360), ofColor(ofRandom(128, 255), ofRandom(128, 255), ofRandom(128, 255)));
  }
}

void SpaceController::addBrick(float elevation, float heading, const ofColor& color) {
    ofPtr<Brick> brick(new Brick);
    brick->setPositionSpherical(_config.domeRadius() + _config.domeMargin(), elevation, heading);
    brick->setSize(_config.brickSize());
    brick->setColor(color);

    _world.addObject(brick.get());
    _state.bricks().push_back(brick);
}

void SpaceController::addBall(float elevation, float heading) {
    ofPtr<Ball> ball(new Ball);
    ball->setSize(ofVec3f(_config.ballRadius(), _config.ballRadius(), _config.ballRadius()));
    auto t = new OrbitalTrajectory();
    t->setRadius(_config.domeRadius() + _config.domeMargin());
    t->setSpeed(0.04);
    t->initWithTwoPoints(elevation, heading, -45, heading + ofRandom(-45, 45));
    ball->setTrajectory(t);
    
    _world.addObject(ball.get());
    _state.balls().push_back(ball);;
}

void SpaceController::addPaddle(float heading, Player* player) {
    ofPtr<Paddle> paddle(new Paddle(player));
    player->setPaddle(paddle.get());
    paddle->setPositionCylindrical(_config.domeRadius() + _config.domeMargin(), heading, _config.domeMargin());
    paddle->setSize(_config.paddleSize());

    _world.addObject(paddle.get());
    _state.paddles().push_back(paddle);
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
      ball->bounce(normal); // maybe not the best method/place
      break;
    case GAME_OBJECT_BALL:
      notifyBallHitBall(ball, static_cast<Ball*>(obj));
      break;
    case GAME_OBJECT_WALL:
      notifyBallHitWall(ball, static_cast<Wall*>(obj));
      break;
    default:
      break;
  }
}

