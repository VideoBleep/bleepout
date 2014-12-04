//
//  BleepoutApp.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "BleepoutApp.h"


void BleepoutApp::setup() {
  _config.reset(new BleepoutConfig());
  // load config....
  ofSetFrameRate(_config->fps());
  ofSetLogLevel(_config->logLevel());
  ofDisableAntiAliasing();
  ofSetVerticalSync(_config->vsync());
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
