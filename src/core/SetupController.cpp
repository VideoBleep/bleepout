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
	// TODO: Remove. This is temporary test code 
	_lobby.push_back(ofPtr<Player>(new Player()));
	_lobby.push_back(ofPtr<Player>(new Player()));
	_lobby.push_back(ofPtr<Player>(new Player()));
	_lobby.push_back(ofPtr<Player>(new Player()));
	_lobby.push_back(ofPtr<Player>(new Player()));
	//...
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
  if (key == OF_KEY_RETURN) {
    if (!tryStartRound()) {
      //...?
    }
  } else if (key >= '0' && key <= '9') {
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

bool SetupController::tryStartRound() {
	if (_lobby.empty()) {
    ofLogError() << "Cannot start round: no players!";
    return false;
  }
  if (!_roundConfig) {
    ofLogError() << "Cannot start round: no round config selected";
    return false;
  }
  notifyStartRound(_roundConfig, _lobby);
  return true;
}

void SetupController::notifyStartRound(ofPtr<RoundConfig> config,
                                        std::list<ofPtr<Player> > players) {
  StartRoundEventArgs e(config, players);
  ofNotifyEvent(startRoundEvent, e);
  logEvent("StartRound", e);
}
