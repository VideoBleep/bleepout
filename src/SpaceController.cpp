//
//  SpaceController.cpp
//  bleepout
//
//  Created by tekt on 12/7/14.
//
//

#include "SpaceController.h"

namespace {
  
  static ofVec2f getPaddleStartPosition(int i, int numPlayers, RoundConfig& config) {
    
    auto HEIGHT = ofGetHeight();
    ofVec2f pos;
    pos.y = ofGetHeight() - (config.brickSize().y);
    float halfWidth = config.brickSize().x;
    pos.x = ofMap((float)i, 0.0f, (float)numPlayers, halfWidth, ofGetWidth() - halfWidth);
    return pos;
  }
  static ofVec2f getBallStartPosition(int i, int numPlayers, RoundConfig& config) {
    ofVec2f pos;
    pos.y = ofGetHeight() / 2;
    float halfWidth = config.brickSize().x;
    pos.x = ofMap((float)i, 0.0f, (float)numPlayers, halfWidth, ofGetWidth() - halfWidth);
    return pos;
  }
  
}


SpaceController::SpaceController(RoundState& state, RoundConfig & config)
: _state(state), _config(config) {
}

void SpaceController::setup() {
  _world.setup();
  _world.disableMousePickingEvents();
  _world.disableGrabbing();
  _world.setGravity(ofVec3f(0,0,0));
  _world.enableCollisionEvents();
  ofAddListener(_world.COLLISION_EVENT, this, &SpaceController::onCollision);  
  
  int numPlayers = _state.players().size();
  for (int i = 0; i < numPlayers; i++) {
    ofPtr<Player> player = _state.players()[i];
    ofVec2f paddleCenter = getPaddleStartPosition(i, numPlayers, _config);
    addPaddle(paddleCenter, player.get());
    ofVec2f ballCenter = getBallStartPosition(i, numPlayers, _config);
    addBall(ballCenter);
  }
  
  _state.dumpToLog();
  //...

}

void SpaceController::addBrick(ofVec3f center) {
    ofPtr<Brick> brick(new Brick);
    brick->init(_config.brickSize().x, _config.brickSize().y, _config.brickSize().y);
    brick->create(_world.world, center, 1.0);
    brick->getRigidBody()->setGravity(btVector3(0, 0, 0));
    brick->setData(brick.get());
    brick->activate();
    brick->add();
    _state.bricks().push_back(brick);
}

btVector3 of2bt(const ofVec3f& v) {
    return btVector3(v.x, v.y, v.z);
}

static void setObjPhysics(ofxBulletBaseShape* obj, PhysicsOptions vals) {
    obj->setFriction(vals.friction);
    // todo: translate to Bullet concepts
    // vals.density
    // vals.bounce
}

void SpaceController::addBall(ofVec3f center) {
    ofPtr<Ball> ball(new Ball);
    ball->init(_config.ballRadius());
    setObjPhysics(ball.get(), _config.ballPhysics());
    ball->create(_world.world, center, 0.00001);
    ball->getRigidBody()->setLinearVelocity(btVector3(30, 200, 0));
    ball->setData(ball.get());
    ball->setActivationState(DISABLE_DEACTIVATION);
    ball->setDamping(0, 0);
    //ball->enableKinematic();
    ball->add();
    _state.balls().push_back(ball);;
}

void SpaceController::addPaddle(ofVec3f center, Player* player) {
    ofPtr<Paddle> paddle(new Paddle(player));
    player->setPaddle(paddle.get());
    paddle->init(_config.paddleSize().x, _config.paddleSize().y, _config.paddleSize().y);
    setObjPhysics(paddle.get(), _config.paddlePhysics());
    paddle->create(_world.world, center, 0.0);
    paddle->setData(paddle.get());
    paddle->setActivationState(DISABLE_DEACTIVATION);
    //paddle->enableKinematic();
    paddle->add();
    _state.paddles().push_back(paddle);
}

void SpaceController::update() {
  _world.update();
}

void SpaceController::onCollision(ofxBulletCollisionData &cdata) {
    if (cdata.userData1 == NULL || cdata.userData2 == NULL)
        return;
    
    GameObject* obj1 = reinterpret_cast<GameObject*>(cdata.userData1);
    GameObject* obj2 = reinterpret_cast<GameObject*>(cdata.userData2);
    
    if (obj1->type() == GAME_OBJECT_BALL) {
        ballHitObject(static_cast<Ball*>(obj1), obj2);
    } else if (obj2->type() == GAME_OBJECT_BALL) {
        ballHitObject(static_cast<Ball*>(obj2), obj1);
    }
}

void SpaceController::ballHitObject(Ball *ball, GameObject *obj) {
  switch (obj->type()) {
    case GAME_OBJECT_BRICK:
      notifyBallHitBrick(ball, static_cast<Brick*>(obj));
      break;
    case GAME_OBJECT_PADDLE:
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

