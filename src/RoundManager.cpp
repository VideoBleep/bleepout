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
                                 RendererBase& renderer,
                                 Physics& physics)
  : _config(config),
    _playerManager(playerManager),
    _renderer(renderer),
    _physics(physics) {
}

RoundController::~RoundController() {
}

void RoundController::setup() {
  ofAddListener(_physics.collisionEvent, this, &RoundController::collisionDetected);
  
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
  brick->setPosition(center);
  brick->setDimensions(_config.brickSize());
  _bricks.push_back(brick);
}

void RoundController::addBall(ofVec3f center) {
  ofPtr<Ball> ball(new Ball);
  //ball->setPhysics(_config.ballDensity(), _config.ballBounce(), _config.ballFriction());
  ball->setPosition(center);
  ball->setRadius(_config.ballRadius());
  ball->setVelocity(_config.ballInitialVelocity());

  _balls.push_back(ball);
}

void RoundController::addPaddle(ofVec3f center, ofPtr<Player> player) {
  ofPtr<Paddle> paddle(new Paddle(player));
  player->setPaddle(paddle);
  //paddle->setPhysics(_config.paddleBounce(), _config.paddleDensity(), _config.paddleFriction());
  paddle->setPosition(center);
  paddle->setDimensions(_config.paddleSize());
  _paddles.push_back(paddle);
}

void RoundController::draw() {
  _renderer.draw(*this);
  //...
}

void RoundController::update() {
  //ofLogVerbose() << "OMG UPDATE!!!";
  for (auto& obj : paddles()) {
    PhysicsObject& o = *obj;
    o.update();
  }
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
        paddle.setPosition(pos);
      }
      break;
    case OF_KEY_RIGHT:
      //...
      if (_paddles.size() > 0) {
        Paddle& paddle = *(_paddles[0]);
        ofVec2f pos = paddle.getPosition();
        pos.x += 5;
        paddle.setPosition(pos);
      }
      break;
      
    default:
      break;
  }
}

void RoundController::setPaddlePosition(GameObjectId playerId, float xPercent) {
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
  pos.x = xPercent * ofGetWidth();
  ofLogVerbose() << "Setting paddle position to " << pos;
  paddle->setPosition(pos);
  //...
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

void RoundController::collisionDetected(CollisionArgs &e) {
  if (e.a == NULL || e.b == NULL)
    return;
  if (e.a->type() == GAME_OBJECT_BALL) {
    ballHitObject(static_cast<Ball&>(*e.a), *e.b);
  } else if (e.b->type() == GAME_OBJECT_BALL) {
    ballHitObject(static_cast<Ball&>(*e.a), *e.b);
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
