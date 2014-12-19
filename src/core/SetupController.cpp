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
  ofTranslate(100, 250);
  ofSetColor(0, 0, 0);
  ofDrawBitmapString("Waiting for players...", 0, 0);
  ofTranslate(0, 15);
  ofDrawBitmapString("Players: " + ofToString(_players.size()), 0, 0);
  if (canStartRound()) {
    ofTranslate(0, 15);
    ofDrawBitmapString("Press 'n' to start round...", 0, 0);
  }
  //...
  ofPopMatrix();
  ofPopStyle();
}

void SetupController::keyPressed(int key) {
  if (key == 'n') {
    // yes it's ugly... creating config, returning it by value,
    // then passing that into an auto-generated copy constructor...
    _roundConfig.reset(new RoundConfig(RoundConfig::createTestConfig()));
    if (!tryStartRound()) {
      //...?
    }
  }
}

bool SetupController::canStartRound() const {
  return !_players.empty() && _roundConfig;
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
