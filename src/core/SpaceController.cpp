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
    if (cdata.a->type() == GAME_OBJECT_BALL) {
        Ball& ball = static_cast<Ball&>(*cdata.a);
        if (cdata.b->type() == GAME_OBJECT_PADDLE) {
            auto paddle = static_cast<Paddle&>(*cdata.b);
            ball.bounce(cdata.normalOnA, paddleTrueHitFactor(cdata.pointOnB, paddle.getSize()));
        } else if (!ball.isLaser() || cdata.b->type() != GAME_OBJECT_BRICK) {
            ball.bounce(cdata.normalOnA);
        }
    } else if (cdata.b->type() == GAME_OBJECT_BALL) {
        Ball& ball = static_cast<Ball&>(*cdata.b);
        if (cdata.a->type() == GAME_OBJECT_PADDLE) {
            auto paddle = static_cast<Paddle&>(*cdata.a);
            ball.bounce(-cdata.normalOnA, paddleTrueHitFactor(cdata.pointOnA, paddle.getSize()));
        } else if (!ball.isLaser() || cdata.a->type() != GAME_OBJECT_BRICK) {
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
