//
//  BleepoutApp.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "BleepoutApp.h"


void BleepoutApp::setup() {
  // load config....
  ofSetFrameRate(_config.fps());
  ofSetLogLevel(_config.logLevel());
  ofSetVerticalSync(_config.vsync());

  RoundConfig roundConfig(_config);
  _roundController.reset(new RoundController(roundConfig));
  _roundController->setup();

  // [jim] May not be in the correct place here, but putting it back temporarily to aid sockets integration
  _playerManager.reset(new PlayerManager(_roundController));
  _playerManager->addPlayer();
	
}

void BleepoutApp::update() {
  if (_roundController) {
    _roundController->update();
  }
}

void BleepoutApp::draw() {
  if (_roundController) {
    _roundController->draw();
  }
}

void BleepoutApp::keyPressed(int key) {
  if (_roundController) {
    _roundController->keyPressed(key);
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
