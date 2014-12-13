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

void SpaceController::setup() {
  _world.setup();
  ofAddListener(_world.collisionEvent, this, &SpaceController::onCollision);
  
  int numPlayers = _state.players().size();
  for (int i = 0; i < numPlayers; i++) {
    ofPtr<Player> player = _state.players()[i];
    addPaddle(360 * i / (numPlayers * 1.0), player.get());
    ofVec2f ballCenter = getBallStartPosition(i, numPlayers, _config);
  }

  for (int i = 0; i < 5; i ++) {
    addBall(30, ofRandom(360));
  }
    
  int cols = 12;
  int rows = 10;
    
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++) {
      float s = i / (cols * 1.0);
      addBrick(30 + 3 * j,
               s * 360 + j * 2 + ((i % 2) ? 5 : -5),
               ofColor(s * 255, j / (rows * 1.0) * 255, (1 - s) * 255));
    }
  }
    
  for (int i = 0; i < 6; i++) {
    addCurvedWall(30, i * 60 + 15, 70, i * 60 + 45, 10);
  }

  addBrickRing(72, ofColor(0, 0, 0), 12);
  addBrickRing(76, ofColor(0, 0, 0), 10);
  addBrickRing(80, ofColor(0, 0, 0), 8);

}

void SpaceController::addBrick(float elevation, float heading, const ofColor& color) {
    ofPtr<Brick> brick(new Brick);
    brick->setPositionSpherical(_config.domeRadius() + _config.domeMargin(), elevation, heading);
    brick->setSize(_config.brickSize());
    brick->setColor(color);

    _world.addObject(brick.get());
    _state.bricks().push_back(brick);
}

void SpaceController::addBrickRing(float elevation, const ofColor& color, int count, float phase) {
    for (int i = 0; i < count; i++) {
        addBrick(elevation, i * 360 / (count * 1.0) + phase, color);
    }
}

void SpaceController::addBall(float elevation, float heading) {
    ofPtr<Ball> ball(new Ball);
    ball->setSize(ofVec3f(_config.ballRadius(), _config.ballRadius(), _config.ballRadius()));
    auto t = new OrbitalTrajectory();
    t->setRadius(_config.domeRadius() + _config.domeMargin());
    t->setSpeed(0.03);
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

void SpaceController::addWall(float elevation, float heading, float width, float height, float depth) {
    ofPtr<Wall> wall(new Wall());
    wall->setPositionSpherical(_config.domeRadius() + _config.domeMargin(), elevation, heading);
    wall->setSize(ofVec3f(width, width, width));
    _world.addObject(wall.get());
    _state.walls().push_back(wall);
}

void SpaceController::addCurvedWall(float elevation1, float heading1, float elevation2, float heading2, float width) {
    float r = _config.domeRadius() + _config.domeMargin();
    float theta = elevation1;
    float phi = heading1;
    float dtheta = elevation2 - elevation1;
    float dphi = heading2 - heading1;
    int steps = floor(max((r * dtheta * PI/180.0) / width, (r * dphi * PI/180.0) / width));
    dtheta /= steps * 1.0;
    dphi /= steps * 1.0;
    for (int i = 0; i < steps; i++) {
        addWall(theta, phi, width, width, width);
        theta += dtheta;
        phi += dphi;
    }
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

