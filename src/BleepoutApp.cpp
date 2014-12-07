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
  _config.reset(new BleepoutConfig());
  _config->loadFile(""); // ugly...
  // load config....
  ofSetFrameRate(_config->fps());
  ofSetLogLevel(_config->logLevel());
  ofDisableAntiAliasing();
  ofSetVerticalSync(_config->vsync());
  _renderer.reset(new SimpleRenderer);
  _playerManager.reset(new PlayerManager);
  _playerManager->addPlayer();
  RoundConfig roundConfig;
  roundConfig.loadFile(""); // this is ugly and should be changed at some point
  _roundController.reset(new RoundController(roundConfig,
                                             *_playerManager,
                                             *_renderer));
  _roundController->setup();
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
