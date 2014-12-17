//
//  SetupController.cpp
//  bleepout
//
//  Created by tekt on 12/16/14.
//
//

#include "SetupController.h"
#include "RoundManager.h"
#include <ofMain.h>

SetupController::SetupController(const BleepoutConfig& appConfig)
: _appConfig(appConfig) { }

void SetupController::setup() {
  _players.push_back(ofPtr<Player>(new Player()));
  //...
}

void SetupController::update() {
  //...
}

void SetupController::draw() {
  ofPushStyle();
  ofPushMatrix();
  ofFill();
  ofSetColor(ofColor::orange);
  int foo = (int)ofGetElapsedTimef() % 30;
  ofRect(50 + foo * 10, 50, 100, 100);
  //...
  ofPopMatrix();
  ofPopStyle();
}

void SetupController::keyPressed(int key) {
  if (key == 'n') {
    // yes it's ugly... creating config, returning it by value,
    // then passing that into an auto-generated copy constructor...
    _roundConfig.reset(new RoundConfig(RoundConfig::createTestConfig(_appConfig)));
    if (!tryStartRound()) {
      //...?
    }
  }
}

bool SetupController::tryStartRound() {
  if (_players.empty()) {
    ofLogError() << "Cannot start round: no players!";
    return false;
  }
  if (!_roundConfig) {
    ofLogError() << "Cannot start round: no round config selected";
    return false;
  }
  notifyStartRound(_roundConfig, _players);
  return true;
}
