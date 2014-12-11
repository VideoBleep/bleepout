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
    addPaddle(2 * PI * i / (numPlayers * 1.0), player.get());
    ofVec2f ballCenter = getBallStartPosition(i, numPlayers, _config);
    addBall(ballCenter);
    addBall(ballCenter);
    addBall(ballCenter);
  }
  //...

}

void SpaceController::addBrick(ofVec3f center) {
    ofPtr<Brick> brick(new Brick);
    _world.addObject(brick.get());
    brick->setPosition(center);
    brick->setSize(_config.brickSize());
    _state.bricks().push_back(brick);
}

static void setObjPhysics(PhysicsObject* obj, PhysicsOptions vals) {
    // todo: reimplement or eliminate?
    // vals.friction
    // vals.density
    // vals.bounce
}

void SpaceController::addBall(ofVec3f center) {
    ofPtr<Ball> ball(new Ball);
    _world.addObject(ball.get());
    ball->setPosition(center);
    ball->setSize(ofVec3f(_config.ballRadius(), _config.ballRadius(), _config.ballRadius()));

    _state.balls().push_back(ball);;
}

void SpaceController::addPaddle(float theta, Player* player) {
    ofPtr<Paddle> paddle(new Paddle(player));
    player->setPaddle(paddle.get());
    _world.addObject(paddle.get());
    paddle->setPositionCylindrical(theta, _config.domeRadius() + _config.domeMargin(), _config.domeMargin());
    paddle->setSize(_config.paddleSize());
    _state.paddles().push_back(paddle);
}

void SpaceController::update() {
    _world.update();
}

void SpaceController::onCollision(CollisionArgs &cdata) {
    if (cdata.a->type() == GAME_OBJECT_BALL) {
        ballHitObject(static_cast<Ball*>(cdata.a), cdata.b);
    } else if (cdata.b->type() == GAME_OBJECT_BALL) {
        ballHitObject(static_cast<Ball*>(cdata.b), cdata.a);
    }
}

void SpaceController::ballHitObject(Ball *ball, GameObject *obj) {
  switch (obj->type()) {
    case GAME_OBJECT_BRICK:
      notifyBallHitBrick(ball, static_cast<Brick*>(obj));
      break;
    case GAME_OBJECT_PADDLE:
      ball->bounce(); // maybe not the best method/place
      notifyBallHitPaddle(ball, static_cast<Paddle*>(obj));
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

