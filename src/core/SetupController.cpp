//
//  SetupController.cpp
//  bleepout
//
//  Created by tekt on 12/16/14.
//
//

#include "SetupController.h"
#include "RoundManager.h"
#include "BleepoutParameters.h"
#include <ofMain.h>

SetupController::SetupController(const BleepoutConfig& appConfig)
: _appConfig(appConfig)
, EventSource() { }

void SetupController::setup() {

}

void SetupController::update() {
  //...
}

// TODO: Are we even calling this anymore? 
void SetupController::draw() {
  ofBackground(ofColor::white);
  ofPushStyle();
  ofPushMatrix();
  {
    ofSetColor(0);
    ofTranslate(100, 250);
    ofDrawBitmapString("Waiting for players...", 0, 0);
    ofTranslate(0, 15);
	ofDrawBitmapString("Players: " + ofToString(_lobby.size()), 0, 0);
    if (canStartRound()) {
      ofTranslate(0, 15);
      ofDrawBitmapString("Press ENTER to start round...", 0, 0);
    }
  }
  ofPopMatrix();
  ofPopStyle();
  ofPushMatrix();
  ofPushStyle();
  {
    ofTranslate(350, 250);
    ofSetColor(0);
    ofDrawBitmapString("Select round configuration:", 0, 0);
    int i = 0;
    for (auto& round : _appConfig.roundConfigs()) {
      ofTranslate(0, 25);
      bool selected = _roundConfig && _roundConfig == round;
      std::string prefix;
      if (selected)
        prefix = "**";
      ofDrawBitmapString(prefix + "[" + ofToString(i) + "]: " + round->name(), 0, 0);
      i++;
    }
  }
  ofPopMatrix();
  ofPopStyle();
}

void SetupController::keyPressed(int key) {
  if (key >= '0' && key <= '9') {
    int i = key - '0';
    if (i < _appConfig.roundConfigs().size()) {
      _roundConfig = _appConfig.roundConfigs()[i];
    }
    //........???!!!
  }
}

bool SetupController::canStartRound() const {
	return !_lobby.empty() && _roundConfig;
}

void SetupController::handlePlayerConnected(PlayerEventArgs& e) {
	_lobby.push_back(ofPtr<Player>(e.player()));
}