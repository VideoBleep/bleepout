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
  _box2d.init();
  _box2d.setGravity(0, 0);
  //  _box2d.createGround();
  _box2d.createBounds(ofRectangle(5, 5, ofGetWidth() - 10, ofGetHeight() -10 ));
  _box2d.setFPS(BleepoutConfig::getInstance().fps());
  _box2d.registerGrabbing();
  _box2d.enableEvents();
  ofAddListener(_box2d.contactStartEvents, this, &RoundController::contactStart);
  ofAddListener(_box2d.contactEndEvents, this, &RoundController::contactEnd);
  
  
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

void RoundController::addBrick(ofVec2f center) {
  ofRectangle rect;
  rect.setFromCenter(center, _config.brickSize().x, _config.brickSize().y);
  ofPtr<Brick> brick(new Brick);
  brick->setup(_box2d.getWorld(), rect);
  brick->setData(brick.get());
  _bricks.push_back(brick);
}

void RoundController::addBall(ofVec2f center) {
  ofPtr<Ball> ball(new Ball);
  ball->setPhysics(_config.ballDensity(), _config.ballBounce(), _config.ballFriction());
  ball->setup(_box2d.getWorld(), center, _config.ballRadius());
  ball->setVelocity(_config.ballInitialVelocity());
  ball->setData(ball.get());
  _balls.push_back(ball);
}

void RoundController::addPaddle(ofVec2f center, ofPtr<Player> player) {
  ofPtr<Paddle> paddle(new Paddle(player));
  ofRectangle rect;
  player->setPaddle(paddle);
  rect.setFromCenter(center, _config.paddleSize().x, _config.paddleSize().y);
  paddle->setup(_box2d.getWorld(), rect);
  paddle->setPhysics(_config.paddleBounce(), _config.paddleDensity(), _config.paddleFriction());
  paddle->setData(paddle.get());
  
  _paddles.push_back(paddle);
}

void RoundController::draw() {
  _renderer.draw(*this);
  //...
}

void RoundController::update() {
  //ofLogVerbose() << "OMG UPDATE!!!";
  _box2d.update();  //...
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
        paddle.setPosition(pos.x - 5, pos.y);
      }
      break;
    case OF_KEY_RIGHT:
      //...
      if (_paddles.size() > 0) {
        Paddle& paddle = *(_paddles[0]);
        ofVec2f pos = paddle.getPosition();
        paddle.setPosition(pos.x + 5, pos.y);
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
//  ofLogVerbose() << "Paddle position was " << pos;
  pos.x = xPercent * ofGetWidth();
//  ofLogVerbose() << "Setting paddle position to " << pos;
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

void RoundController::contactStart(ofxBox2dContactArgs &e) {
  if (e.a == NULL || e.b == NULL)
    return;
  GameObject* objA = (GameObject*)e.a->GetBody()->GetUserData();
  GameObject* objB = (GameObject*)e.b->GetBody()->GetUserData();
  if (!objA || !objB) {
    ofLogVerbose() << "Unable to extra game object from b2d body";
    return;
  }
  if (objA->type() == GAME_OBJECT_BALL) {
    ballHitObject(static_cast<Ball&>(*objA), *objB);
  } else if (objB->type() == GAME_OBJECT_BALL) {
    ballHitObject(static_cast<Ball&>(*objB), *objA);
  }
}

void RoundController::contactEnd(ofxBox2dContactArgs &e) {
  //...
}

void RoundController::ballHitObject(Ball &ball, GameObject &obj) {
  ofLogVerbose() << "pre-event: ball hit something: " << ball << " " << obj;
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
  notifyBallHitBrick(ball, brick);
}

void RoundController::ballHitPaddle(Ball &ball, Paddle &paddle) {
  ball.setLastPlayer(paddle.player());
  notifyBallHitPaddle(ball, paddle);
}

void RoundController::dumpToLog() {
  ofLogVerbose() << "--BEGIN round state--";
  _paddles.dumpToLog("Paddles");
  _balls.dumpToLog("Balls");
  _bricks.dumpToLog("Bricks");
  _playerManager.players().dumpToLog("Players");
  ofLogVerbose() << "--  END round state--";
}
