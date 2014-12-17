//
//  SetupController.cpp
//  bleepout
//
//  Created by tekt on 12/16/14.
//
//

#include "SetupController.h"
#include "RoundManager.h"

SetupController::SetupController(const BleepoutConfig& appConfig)
: _appConfig(appConfig) { }

void SetupController::setup() {
  //...
}

void SetupController::update() {
  //...
}

void SetupController::draw() {
  //...
}

bool SetupController::tryStartRound(ofPtr<RoundController>& roundController) {
  //...
  return false;
}
