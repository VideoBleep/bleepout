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

SetupController::SetupController()
: EventSource() { }

void SetupController::setup() {

}

void SetupController::update() {
  //...
}

bool SetupController::canStartRound() const {
	return !_lobby.empty();
}

void SetupController::handlePlayerConnected(PlayerEventArgs& e) {
	_lobby.push_back(ofPtr<Player>(e.player()));
}