//
//  RoundManager.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "RoundManager.h"
#include "BleepoutConfig.h"

RoundController::RoundController(RoundConfig config,
                                 ofPtr<PlayerManager> playerManager)
: _config(config), _playerManager(playerManager) { }

void RoundController::setup() {
  _box2d.init();
  _box2d.createGround();
  _box2d.setFPS(BleepoutConfig::getInstance().fps());
  //...
}

void RoundController::draw() {
  //...
}

void RoundController::update() {
  //...
}
