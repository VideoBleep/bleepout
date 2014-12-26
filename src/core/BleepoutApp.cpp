//
//  BleepoutApp.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "BleepoutApp.h"

BleepoutApp::BleepoutApp()
: _config()
, EventSource()
, _endingRound(false) { }

void BleepoutApp::setup() {
  enableLogging(OF_LOG_NOTICE); // this is only for app-level events
  _config.reset(BleepoutConfig::createConfig());
  _appParams.reset(new BleepoutParameters(*_config));
  ofSetFrameRate(_config->fps());
  ofSetLogLevel(_config->logLevel());
  ofSetVerticalSync(_config->vsync());
  ofSetBackgroundAuto(false);
  
  _setupController.reset(new SetupController(*_config));
  _setupController->setup();
  ofAddListener(_setupController->tryStartRoundEvent, this,
                &BleepoutApp::onTryStartRound);
  
  _adminController.reset(new AdminController(*_appParams, *_setupController));
  _adminController->setup();
  _adminController->attachTo(*this);
  ofAddListener(_adminController->tryStartRoundEvent, this, &BleepoutApp::onTryStartRound);
  ofAddListener(_adminController->tryEndRoundEvent, this, &BleepoutApp::onTryEndRound);

  _playerController.reset(new PlayerController());

  _playerManager.reset(new PlayerManager(*_playerController));
  _playerManager->setup();
  // Temporary, I believe
  _playerManager->addPlayer();


  // Handle playerCreate event
  ofAddListener(_playerController->playerConnectedEvent, _setupController.get(), &SetupController::handlePlayerConnected); 
  //ofAddListener(_playerController->playerAddedEvent, _setupController.get(), &SetupController::handlePlayerAdded);

#ifdef ENABLE_SYPHON
  _syphonClient.setup();
  _syphonEnabled = false;
#endif // ENABLE_SYPHON
}

void BleepoutApp::update() {
#ifdef ENABLE_SYPHON
  if (!_syphonEnabled && _appParams->enableSyphon) {
    _syphonEnabled = true;
    _syphonClient.set(_appParams->syphonServerName,
                      _appParams->syphonAppName);
  }
  _syphonEnabled = _appParams->enableSyphon;
#endif
  _adminController->update();
  if (_roundController) {
    _roundController->update();
  } else if (_setupController) {
    _setupController->update();
  }
  if (_endingRound) {
    endRound();
  }
}

void BleepoutApp::draw() {
  ofBackground(0, 0, 0);
#ifdef ENABLE_SYPHON
  if (_syphonEnabled)
    _syphonClient.draw(0, 0, ofGetWidth(), ofGetHeight());
#endif // ENABLE_SYPHON
  if (_roundController) {
   _roundController->draw();
  } else if (_setupController) {
    _setupController->draw();
  }
  _adminController->draw();
}

void BleepoutApp::onTryStartRound(StartRoundEventArgs &e) {
  if (!e.config() || e.players().empty()) {
    ofLogWarning() << "Cannot start round: " << e;
    return;
  }
  if (_roundController) {
    ofRemoveListener(_roundController->tryEndRoundEvent, this,
                     &BleepoutApp::onTryEndRound);
    ofLogError() << "Round has already been started";
    return;
  }
  _playerManager->setIsInRound(true);
  _appParams->setCurrentRound(e.config()->name());
  _roundController.reset(new RoundController(*e.config(),
                                             *_appParams,
                                             e.players(),
                                             *_playerManager));
  _roundController->setup();
  ofAddListener(_roundController->tryEndRoundEvent, this,
                &BleepoutApp::onTryEndRound);
  ofAddListener(_roundController->roundQueueEvent, _playerController.get(),
	  &PlayerController::onRoundQueue);
  e.markHandled();
  notifyRoundStarted(_roundController->state());
}

void BleepoutApp::onTryEndRound(EndRoundEventArgs &e) {
  if (!_roundController) {
    ofLogError() << "Round was not active";
    return;
  }
  _endingRound = true;
  e.markHandled();
}

void BleepoutApp::endRound() {
  _playerManager->setIsInRound(false);
  _roundController.reset();
  _endingRound = true;
  notifyRoundEnded();
}

void BleepoutApp::notifyRoundStarted(RoundState &state) {
  RoundStateEventArgs e(state);
  ofNotifyEvent(roundStartedEvent, e);
  logEvent("RoundStarted", e);
}

void BleepoutApp::notifyRoundEnded() {
  EmptyEventArgs e;
  ofNotifyEvent(roundEndedEvent, e);
  logEvent("RoundEnded", e);
}

void BleepoutApp::keyPressed(int key) {
  _adminController->keyPressed(key);
  if (_roundController) {
    _roundController->keyPressed(key);
  } else if (_setupController) {
    _setupController->keyPressed(key);
  }
}

void BleepoutApp::mousePressed(int x, int y, int button) {
    if (_roundController) {
        _roundController->mousePressed(x, y, button);
    }
}

void BleepoutApp::mouseMoved(int x, int y) {
  if (_roundController) {
    _roundController->mouseMoved(x, y);
  }
}

void BleepoutApp::mouseReleased(int x, int y, int button) {
    if (_roundController) {
        _roundController->mouseReleased(x, y, button);
    }
}

void BleepoutApp::mouseDragged(int x, int y, int button) {
  if (_roundController) {
    _roundController->mouseDragged(x, y, button);
  }
}
