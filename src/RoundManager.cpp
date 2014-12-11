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
#include "DomeRenderer.h"
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
  
  _renderer.reset(new DomeRenderer());
  _renderer->setup();
  
  ofLog(OF_LOG_NOTICE) << _state;
  //...
}

void RoundController::draw() {
  _renderer->draw(_state, _config);
  //...
}

void RoundController::update() {
  //ofLogVerbose() << "OMG UPDATE!!!";

  _spaceController->update();
  _logicController->update();
}

void RoundController::keyPressed(int key) {
  if (ofGetKeyPressed(OF_KEY_COMMAND)) {
    _renderer->keyPressed(key);
  } else {
    if (key == 'l') {
      dumpToLog(OF_LOG_NOTICE);
    }
  }
}

void RoundController::setPaddlePosition(PlayerYawPitchRollMessage ypr) {
	Paddle* paddle = ypr.player->paddle();
	if (!paddle) {
		ofLogError() << "Unable to set paddle position for player: " << ypr.player->id();
		return;
	}

    paddle->setPositionCylindrical(_config.domeRadius() + _config.domeMargin(), 2 * PI * ypr.yaw, _config.domeMargin());
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
  
  paddle->setPositionCylindrical(_config.domeRadius() + _config.domeMargin(), 2 * PI * xPercent, _config.domeMargin());
}

void RoundController::mousePressed(int x, int y, int button) {
  if (ofGetKeyPressed(OF_KEY_COMMAND)) {
    _renderer->mousePressed(x, y, button);
  } else {
        
  }
}

void RoundController::mouseMoved(int x, int y) {
  if (ofGetKeyPressed(OF_KEY_COMMAND)) {
        _renderer->mouseMoved(x, y);
  } else if (_state.players().size()) {
    ofPtr<Player> player = _state.players()[0];
    setPaddlePosition(player->id(), (float)x / ofGetWidth());
  }
}

void RoundController::mouseDragged(int x, int y, int button) {
  if (ofGetKeyPressed(OF_KEY_COMMAND)) {
    _renderer->mouseDragged(x, y, button);
  } else {
  
  }
}

void RoundController::mouseReleased(int x, int y, int button) {
  if (ofGetKeyPressed(OF_KEY_COMMAND)) {
    _renderer->mouseReleased(x, y, button);
  } else {
    
  }
}


void RoundController::dumpToLog(ofLogLevel level) {
  ofLog(level) << "--BEGIN round state--";
  ofLog(level) << _state;
  ofLog(level) << "--  END round state--";
}
