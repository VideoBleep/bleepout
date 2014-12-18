//
//  BleepoutApp.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "BleepoutApp.h"

BleepoutApp::BleepoutApp()
: _config() { }

void BleepoutApp::setup() {
  // load config....
  
  _config = BleepoutConfig::createTestConfig();
  ofSetFrameRate(_config.fps());
  ofSetLogLevel(_config.logLevel());
  ofSetVerticalSync(_config.vsync());
  ofSetBackgroundAuto(false);
  
  _setupController.reset(new SetupController(_config));
  _setupController->setup();
  ofAddListener(_setupController->startRoundEvent, this,
                &BleepoutApp::onStartRound);

  _playerManager.reset(new PlayerManager());
  _playerManager->setup();
  _playerManager->addPlayer();
  
#ifdef ENABLE_SYPHON
  _syphonClient.setup();
  _syphonClient.set(_config.syphonServerName(), _config.syphonAppName());
#endif // ENABLE_SYPHON
}

void BleepoutApp::update() {
  if (_roundController) {
    _roundController->update();
  } else if (_setupController) {
    _setupController->update();
  }
}

void BleepoutApp::draw() {
  ofBackground(0, 0, 0);
#ifdef ENABLE_SYPHON
  _syphonClient.draw(0, 0, ofGetWidth(), ofGetHeight());
#endif // ENABLE_SYPHON
  if (_roundController) {
   _roundController->draw();
  } else if (_setupController) {
    _setupController->draw();
  }
}

void BleepoutApp::onStartRound(StartRoundEventArgs &e) {
  if (_roundController) {
    ofRemoveListener(_roundController->roundEndedEvent, this,
                     &BleepoutApp::onRoundEnded);
    ofLogError() << "Round has already been started";
    return;
  }
  _playerManager->setIsInRound(true);
  _roundController.reset(new RoundController(*e.config(),
                                             e.players(),
                                             *_playerManager));
  _roundController->setup();
  ofAddListener(_roundController->roundEndedEvent, this,
                &BleepoutApp::onRoundEnded);
}

void BleepoutApp::onRoundEnded(RoundStateEventArgs &e) {
  if (!_roundController) {
    ofLogError() << "Round was not active";
    return;
  }
  _playerManager->setIsInRound(false);
  _roundController.reset();
}

void BleepoutApp::keyPressed(int key) {
  if (_roundController) {
    _roundController->keyPressed(key);
  } else if (_setupController) {
    _setupController->keyPressed(key);
  }
}

void BleepoutApp::mousePressed(int x, int y, int button) {
    if (_roundController) {
        _roundController->mousePressed(x, y, button);
    }
}

void BleepoutApp::mouseMoved(int x, int y) {
  if (_roundController) {
    _roundController->mouseMoved(x, y);
  }
}

void BleepoutApp::mouseReleased(int x, int y, int button) {
    if (_roundController) {
        _roundController->mouseReleased(x, y, button);
    }
}

void BleepoutApp::mouseDragged(int x, int y, int button) {
  if (_roundController) {
    _roundController->mouseDragged(x, y, button);
  }
}
