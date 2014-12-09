//
//  BleepoutApp.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "BleepoutApp.h"
#include "SimpleRenderer.h"


void BleepoutApp::setup() {
  _config.loadFile(""); // ugly...
  // load config....
  ofSetFrameRate(_config.fps());
  ofSetLogLevel(_config.logLevel());
  ofDisableAntiAliasing();
  ofSetVerticalSync(_config.vsync());

  RoundConfig roundConfig(_config);
  roundConfig.loadFile(""); // this is ugly and should be changed at some point
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

void BleepoutApp::mouseMoved(int x, int y) {
  if (_roundController) {
    _roundController->mouseMoved(x, y);
  }
}
void BleepoutApp::mouseDragged(int x, int y, int button) {
  if (_roundController) {
    _roundController->mouseDragged(x, y, button);
  }
}
