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

namespace {
    
  static ofVec3f getBallStartPosition(int i, int numPlayers,
                                      const RoundConfig& config) {
    return ofVec3f(0, config.domeRadius() + config.domeMargin());
  }
  
}

SpaceController::SpaceController(RoundState& state,
                                 const RoundConfig& config,
                                 const BleepoutParameters& appParams)
: RoundComponent(state, config, appParams) {
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

bool SpaceController::shouldBounce(const Ball &ball, const GameObject &object) const {
    if (object.type() != GAME_OBJECT_BRICK)
        return true;
    if (ball.isLaser() || _appParams.allLasers)
        return false;
    return true;
}

void SpaceController::onCollision(CollisionArgs &cdata) {
    ofVec3f normal = cdata.normal;
    if (cdata.a->type() == GAME_OBJECT_BALL) {
      Ball& ball = static_cast<Ball&>(*cdata.a);
        if (shouldBounce(ball, *cdata.b))
            ball.bounce(cdata.normal);
    } else if (cdata.b->type() == GAME_OBJECT_BALL) {
        Ball& ball = static_cast<Ball&>(*cdata.b);
        if (shouldBounce(ball, *cdata.a))
            ball.bounce(-cdata.normal);
    }
    notifyCollision(cdata.a, cdata.b);
}

void SpaceController::notifyCollision(GameObject *a, GameObject *b) {
  CollisionEventArgs e(a, b);
  ofNotifyEvent(collisionEvent, e);
  logEvent("Collision", e);
}
