//
//  RoundManager.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "Common.h"
#include "RoundManager.h"
#include "BleepoutConfig.h"
#include "RendererBase.h"

namespace {
  
  static ofVec2f getPaddleStartPosition(int i, int numPlayers, RoundConfig& config) {
    
    auto HEIGHT = ofGetHeight();
    ofVec2f pos;
    pos.y = ofGetHeight() - (config.brickSize().y);
    float halfWidth = config.brickSize().x;
    pos.x = ofMap((float)i, 0.0f, (float)numPlayers, halfWidth, ofGetWidth() - halfWidth);
    //...
//    throw NotImplementedException("getPaddleStartPosition");
    return pos;
  }
  static ofVec2f getBallStartPosition(int i, int numPlayers, RoundConfig& config) {
    ofVec2f pos;
    pos.y = ofGetHeight() / 2;
    float halfWidth = config.brickSize().x;
    pos.x = ofMap((float)i, 0.0f, (float)numPlayers, halfWidth, ofGetWidth() - halfWidth);
    //...
//    throw NotImplementedException("getBallStartPosition");
    return pos;
  }
  
}


RoundController::RoundController(RoundConfig config,
                                 PlayerManager& playerManager,
                                 RendererBase& renderer)
  : _config(config),
    _playerManager(playerManager),
    _renderer(renderer) {
}

RoundController::~RoundController() {
}

void RoundController::setup() {
  _world.setup();
  _world.disableMousePickingEvents();
  _world.disableGrabbing();
  _world.enableCollisionEvents();
  _world.setGravity( ofVec3f(0, 25., 0) );
  ofAddListener(_world.COLLISION_EVENT, this, &RoundController::onCollision);
  
  int numPlayers = _playerManager.players().size();
  for (int i = 0; i < numPlayers; i++) {
    ofPtr<Player> player = _playerManager.players()[i];
    ofVec2f paddleCenter = getPaddleStartPosition(i, numPlayers, _config);
    addPaddle(paddleCenter, player);
    ofVec2f ballCenter = getBallStartPosition(i, numPlayers, _config);
    addBall(ballCenter);
  }
  
  //...
}

void RoundController::addBrick(ofVec3f center) {
  ofPtr<Brick> brick(new Brick);
  brick->init(_config.brickSize().x, _config.brickSize().y, _config.brickSize().y);
  brick->create(_world.world, center, 1.0);
  brick->getRigidBody()->setGravity(btVector3(0, 0, 0));
  brick->setData(&*brick);
  brick->activate();
  brick->add();
  _bricks.push_back(brick);
}

btVector3 of2bt(const ofVec3f& v) {
    return btVector3(v.x, v.y, v.z);
}

void RoundController::addBall(ofVec3f center) {
  ofPtr<Ball> ball(new Ball);
  ball->init(_config.ballRadius());
  ball->setFriction(_config.ballFriction());
  ball->create(_world.world, center, 1.0);
  ball->getRigidBody()->setLinearVelocity(of2bt(_config.ballInitialVelocity()));
  ball->setData(&*ball);
  ball->setActivationState(DISABLE_DEACTIVATION);
  //ball->enableKinematic();
  ball->add();
  _balls.push_back(ball);
}

void RoundController::addPaddle(ofVec3f center, ofPtr<Player> player) {
  ofPtr<Paddle> paddle(new Paddle(player));
  player->setPaddle(paddle);
  paddle->init(_config.paddleSize().x, _config.paddleSize().y, _config.paddleSize().y);
  paddle->setFriction(_config.paddleFriction());
  paddle->create(_world.world, center, 1.0);
  paddle->setData(&*paddle);
  paddle->setActivationState(DISABLE_DEACTIVATION);
  paddle->enableKinematic();
  paddle->add();
  _paddles.push_back(paddle);
}

void RoundController::draw() {
  _renderer.draw(*this);
  //...
}

void RoundController::update() {
  //ofLogVerbose() << "OMG UPDATE!!!";
  for (auto& ball : balls()) {
      // update orbital path
  }
  _world.update();
}

void RoundController::keyPressed(int key) {
  if (key == 'l') {
    dumpToLog();
  }
  switch (key) {
    case 'l':
      dumpToLog();
      break;
    case OF_KEY_LEFT:
      //...
      if (_paddles.size() > 0) {
        Paddle& paddle = *(_paddles[0]);
        ofVec2f pos = paddle.getPosition();
        pos.x -= 5;
        //paddle.setPosition(pos);
      }
      break;
    case OF_KEY_RIGHT:
      //...
      if (_paddles.size() > 0) {
        Paddle& paddle = *(_paddles[0]);
        ofVec2f pos = paddle.getPosition();
        pos.x += 5;
        //paddle.setPosition(pos);
      }
      break;
      
    default:
      break;
  }
}

void RoundController::setPaddlePosition(GameObjectId playerId, float radians) {
  ofPtr<Player> player = _playerManager.players().getById(playerId);
  if (!player) {
    ofLogError() << "Player not found: " << playerId;
    return;
  }

  ofPtr<Paddle> paddle = player->paddle();
  if (!paddle) {
    ofLogError() << "Unable to set paddle position for player: " << playerId;
    return;
  }
  
  ofVec2f pos = paddle->getPosition();
  ofLogVerbose() << "Paddle position was " << pos;
  // TODO: calculate
  //ofLogVerbose() << "Setting paddle position to " << pos;
  //paddle->setPosition(pos);
}

void RoundController::mouseMoved(int x, int y) {
  //...
  if (_playerManager.players().size()) {
    ofPtr<Player> player = *(_playerManager.players().begin());
    setPaddlePosition(player->id(), (float)x / ofGetWidth());
  }
}

void RoundController::mouseDragged(int x, int y, int button) {
  //...
}

void RoundController::onCollision(ofxBulletCollisionData &cdata) {
  if (cdata.userData1 == NULL || cdata.userData2 == NULL)
      return;
    
  GameObject* obj1 = reinterpret_cast<GameObject*>(cdata.userData1);
  GameObject* obj2 = reinterpret_cast<GameObject*>(cdata.userData2);
    
  if (obj1->type() == GAME_OBJECT_BALL) {
    ballHitObject(static_cast<Ball&>(*obj1), *obj2);
  } else if (obj2->type() == GAME_OBJECT_BALL) {
    ballHitObject(static_cast<Ball&>(*obj2), *obj1);
  }
}

void RoundController::ballHitObject(Ball& ball, GameObject& obj) {
  switch (obj.type()) {
    case GAME_OBJECT_BRICK:
      ballHitBrick(ball, static_cast<Brick&>(obj));
      break;
    case GAME_OBJECT_PADDLE:
      ballHitPaddle(ball, static_cast<Paddle&>(obj));
      break;
    case GAME_OBJECT_BALL:
      // ????? insanity ensues
      break;
    default:
      break;
  }
}

void RoundController::ballHitBrick(Ball &ball, Brick &brick) {
  //...
  //notifyBallHitBrick(
}

void RoundController::ballHitPaddle(Ball &ball, Paddle &paddle) {
  ball.setLastPlayer(paddle.player());
}

void RoundController::dumpToLog() {
  ofLogVerbose() << "--BEGIN round state--";
  _paddles.dumpToLog("Paddles");
  _balls.dumpToLog("Balls");
  _bricks.dumpToLog("Bricks");
  _playerManager.players().dumpToLog("Players");
  ofLogVerbose() << "--  END round state--";
}
