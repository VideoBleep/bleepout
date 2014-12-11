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
#include "SimpleRenderer.h"
#include "Logging.h"

RoundController::RoundController(RoundConfig config)
  : _config(config) {
}

RoundController::~RoundController() {
}

void RoundController::setup() {
  //...
  _state.players().push_back(ofPtr<Player>(new Player()));
  
  _spaceController.reset(new SpaceController(_state, _config));
  _logicController.reset(new LogicController(_state, _config));
  _spaceController->setup();
  _logicController->setup();
  _logicController->enableLogging(OF_LOG_NOTICE);
  
  _spaceController->enableLogging(OF_LOG_NOTICE);
  _spaceController->attachListener(*_logicController);
  
  _renderer.reset(new SimpleRenderer());
  
  ofLog(OF_LOG_NOTICE) << _state;
  //...
}

void RoundController::draw() {
    ofBackground(255, 255, 255);
  _renderer->draw(_state);
  //_spaceController->drawDebug();
  //...
}

void RoundController::update() {
  //ofLogVerbose() << "OMG UPDATE!!!";

  _spaceController->update();
  _logicController->update();
}

void RoundController::keyPressed(int key) {
  if (key == 'l') {
    dumpToLog(OF_LOG_NOTICE);
  }
}

void RoundController::setPaddlePosition(PlayerYawPitchRollMessage ypr) {
	Paddle* paddle = ypr.player->paddle();
	if (!paddle) {
		ofLogError() << "Unable to set paddle position for player: " << ypr.player->id();
		return;
	}

	ofVec3f pos = paddle->getPosition();
	pos.x = ypr.yaw * ofGetWidth();
	paddle->setPosition(pos);
}

void RoundController::setPaddlePosition(GameObjectId playerId, float xPercent) {
  ofPtr<Player> player = _state.players().getById(playerId);
  if (!player) {
    ofLogError() << "Player not found: " << playerId;
    return;
  }

  Paddle* paddle = player->paddle();
  if (!paddle) {
    ofLogError() << "Unable to set paddle position for player: " << playerId;
    return;
  }
  
  ofVec3f pos = paddle->getPosition();
  pos.x = xPercent * ofGetWidth();
  paddle->setPosition(pos);
  //...
}

void RoundController::mouseMoved(int x, int y) {
  //...
  if (_state.players().size()) {
    ofPtr<Player> player = _state.players()[0];
    setPaddlePosition(player->id(), (float)x / ofGetWidth());
  }
}

void RoundController::mouseDragged(int x, int y, int button) {
  //...
}

void RoundController::dumpToLog(ofLogLevel level) {
  ofLog(level) << "--BEGIN round state--";
  ofLog(level) << _state;
  ofLog(level) << "--  END round state--";
}
