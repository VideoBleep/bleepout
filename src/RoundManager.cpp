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
    
  _state.message.text = "START";
  _state.message.color = ofColor(255, 0, 0);
    
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
    _state.time = ofGetElapsedTimef();

    if (_state.time < 4) {
        _state.message = RoundMessage("VideoBleep\n presents", ofColor(0, 0, 0), 12);
    } else if (_state.time < 8) {
        _state.message = RoundMessage("BLEEP*OUT", ofColor(0, 120, 240), 50, 4);
    } else if (_state.time < 12) {
        _state.message = RoundMessage("STAGE 1 START", ofColor(0, 255, 0), 25);
    } else {
        _state.message.text = "";
    }

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

    paddle->setPositionCylindrical(_config.domeRadius() + _config.domeMargin(), 360 * ypr.yaw, _config.domeMargin());
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
  
  paddle->setPositionCylindrical(_config.domeRadius() + _config.domeMargin(), 360 * xPercent, _config.domeMargin());
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
