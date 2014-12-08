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
  _box2d.init();
  _box2d.setGravity(0, 0);
  //  _box2d.createGround();
  _box2d.createBounds(ofRectangle(5, 5, ofGetWidth() - 10, ofGetHeight() -10 ));
  _box2d.setFPS(_config.appConfig().fps());
  _box2d.registerGrabbing();
  _box2d.enableEvents();
  ofAddListener(_box2d.contactStartEvents, this, &SpaceController::contactStart);
  ofAddListener(_box2d.contactEndEvents, this, &SpaceController::contactEnd);
  
  
  int numPlayers = _state.players().size();
  for (int i = 0; i < numPlayers; i++) {
    ofPtr<Player> player = _state.players()[i];
    ofVec2f paddleCenter = getPaddleStartPosition(i, numPlayers, _config);
    addPaddle(paddleCenter, player);
    ofVec2f ballCenter = getBallStartPosition(i, numPlayers, _config);
    addBall(ballCenter);
  }
  
  _state.dumpToLog();
  //...

}

void SpaceController::addBrick(ofVec2f center) {
  ofRectangle rect;
  rect.setFromCenter(center, _config.brickSize().x, _config.brickSize().y);
  ofPtr<Brick> brick(new Brick);
  brick->setup(_box2d.getWorld(), rect);
  brick->setData(brick.get());
  _state.bricks().push_back(brick);
}

static void setObjPhysics(ofxBox2dBaseShape* obj, PhysicsOptions vals) {
  obj->setPhysics(vals.density, vals.bounce, vals.friction);
}

void SpaceController::addBall(ofVec2f center) {
  ofPtr<Ball> ball(new Ball);
  setObjPhysics(ball.get(), _config.ballPhysics());
  ball->setup(_box2d.getWorld(), center, _config.ballRadius());
  ball->setVelocity(_config.ballInitialVelocity());
  ball->setData(ball.get());
  _state.balls().push_back(ball);
}

void SpaceController::addPaddle(ofVec2f center, ofPtr<Player> player) {
  ofPtr<Paddle> paddle(new Paddle(player));
  ofRectangle rect;
  player->setPaddle(paddle);
  rect.setFromCenter(center, _config.paddleSize().x, _config.paddleSize().y);
  setObjPhysics(paddle.get(), _config.paddlePhysics());
  paddle->setup(_box2d.getWorld(), rect);
  paddle->setData(paddle.get());
  
  _state.paddles().push_back(paddle);
}

void SpaceController::update() {
  _box2d.update();
}

void SpaceController::contactStart(ofxBox2dContactArgs &e) {
  if (e.a == NULL || e.b == NULL)
    return;
  GameObject* objA = (GameObject*)e.a->GetBody()->GetUserData();
  GameObject* objB = (GameObject*)e.b->GetBody()->GetUserData();
  if (!objA || !objB) {
    ofLogVerbose() << "Unable to extract game object from b2d body";
    return;
  }
  if (objA->type() == GAME_OBJECT_BALL) {
    ballHitObject(static_cast<Ball*>(objA), objB);
  } else if (objB->type() == GAME_OBJECT_BALL) {
    ballHitObject(static_cast<Ball*>(objB), objA);
  }
}

void SpaceController::contactEnd(ofxBox2dContactArgs &e) {
  //...
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
    default:
      break;
  }
}

