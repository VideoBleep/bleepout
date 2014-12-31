//
//  BleepoutApp.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "BleepoutApp.h"
#include "Animations.h"
#include "BleepoutParameters.h"

BleepoutApp::BleepoutApp()
: _config()
, EventSource() { }

BleepoutApp::~BleepoutApp() {
}

void BleepoutApp::setup() {
  enableLogging(OF_LOG_NOTICE); // this is only for app-level events
  BleepoutConfig* cfg = BleepoutConfig::loadFromFile("config/bleepoutConfig.json");
  if (!cfg) {
    ofLogWarning() << "Unable to load config file: config/bleepoutConfig.json";
    cfg = BleepoutConfig::createConfig();
  }
  _config.reset(cfg);
  BleepoutParameters::initializeConfig(*_config);
//  _config->saveJsonFile("config/bleepoutConfig.json");
//  for (auto& round : _config->roundConfigs()) {
//    std::string filepath = "config/" + round->name() + ".json";
//    round->saveJsonFile(filepath);
//  }
  ofSetFrameRate(_config->fps());
  ofSetLogLevel(_config->logLevel());
  ofSetVerticalSync(_config->vsync());
  ofSetBackgroundAuto(false);
  
  _setupController.reset(new SetupController(*_config));
  _setupController->setup();
  ofAddListener(_setupController->tryStartRoundEvent, this,
                &BleepoutApp::onTryStartRound);
  
  _adminController.reset(new AdminController(*_setupController));
  _adminController->setup();
  _adminController->attachTo(*this);
  ofAddListener(_adminController->tryStartRoundEvent, this, &BleepoutApp::onTryStartRound);
  
  _audioManager.reset(new AudioManager());
  _audioManager->setup();
  _audioManager->attachTo(*this);

  _playerController.reset(new PlayerController(*_setupController));
  
  _playerManager.reset(new PlayerManager(*this, *_playerController));
  _playerManager->setup();
  // Temporary, I believe
  Player* testPlayer = new Player();
  testPlayer->setColor(ofColor::green);
  _setupController->lobby().push_back(ofPtr<Player>(testPlayer));
//  _playerManager->addPlayer();
  
  
  // Handle playerCreate event
  ofAddListener(_playerController->playerConnectedEvent, _setupController.get(), &SetupController::handlePlayerConnected);
  //ofAddListener(_playerController->playerAddedEvent, _setupController.get(), &SetupController::handlePlayerAdded);
  
  _animationManager.reset(new AppAnimationManager(*this));
  
#ifdef ENABLE_SYPHON
  _syphonClient.setup();
  _syphonEnabled = false;
#endif // ENABLE_SYPHON
}

void BleepoutApp::update() {
  auto& appParams = BleepoutParameters::get();
#ifdef ENABLE_SYPHON
  if (!_syphonEnabled && appParams.enableSyphon) {
    _syphonEnabled = true;
    _syphonClient.set(appParams.syphonServerName,
                      appParams.syphonAppName);
  }
  _syphonEnabled = appParams.enableSyphon;
#endif
  _adminController->update();
  _audioManager->update();
  if (_roundController) {
    _roundController->update();
  } else if (_setupController) {
    _setupController->update();
  }
  _timedActions.update(ofGetElapsedTimef());
}

void BleepoutApp::draw() {
#ifndef RADOME
  ofBackground(0, 0, 0);
#endif
  
#ifdef ENABLE_SYPHON
  if (_syphonEnabled)
    _syphonClient.draw(0, 0, ofGetWidth(), ofGetHeight());
#endif // ENABLE_SYPHON
  
  if (_roundController) {
    _roundController->draw();
  } else if (_setupController) {
    _setupController->draw();
  }
  
#ifndef RADOME
  _adminController->draw();
#endif
  
  for (auto& animation : _animations) {
    if (animation && animation->alive() && animation->visible()) {
      animation->draw();
    }
  }
  
}

void BleepoutApp::onTryStartRound(StartRoundEventArgs &e) {
  auto& appParams = BleepoutParameters::get();
  if (e.configs().empty() || e.players().empty()) {
    ofLogWarning() << "Cannot start round: " << e;
    return;
  }
  if (_roundController) {
    ofLogError() << "Round has already been started";
    return;
  }
  _playerManager->setIsInRound(true);
  _roundController.reset(new RoundController(e.configs(),
                                             e.players(),
                                             *_playerManager));
  _roundController->setup();
  ofAddListener(_roundController->roundEndedEvent, this,
                &BleepoutApp::onRoundEnded);
  ofAddListener(_roundController->roundPlayEvent, this,
                &BleepoutApp::onRoundPlay);
  //ofAddListener(_roundController->roundQueueEvent, _playerController.get(),
  //              &PlayerController::onRoundQueue);
  _audioManager->attachTo(*_roundController);
  _roundController->attachTo(*_adminController);
  e.markHandled();
  notifyRoundStarted(_roundController->state());
}

const RoundConfig* BleepoutApp::currentRoundConfig() const {
  if (_roundController)
    return &_roundController->config();
  return NULL;
}

void BleepoutApp::onRoundEnded(RoundEndedEventArgs &e) {
  _playerManager->setIsInRound(false);
  ofRemoveListener(_roundController->roundEndedEvent, this,
                   &BleepoutApp::onRoundEnded);
  //ofRemoveListener(_roundController->roundQueueEvent, _playerController.get(), &PlayerController::onRoundQueue);
  ofRemoveListener(_roundController->roundPlayEvent, this,
                   &BleepoutApp::onRoundPlay);
  _audioManager->detachFrom(*_roundController);
  _roundController->detachFrom(*_adminController);
  _roundController.reset();
  notifyRoundEnded(e);
}

void BleepoutApp::onRoundPlay(RoundStateEventArgs &e) {
  notifyRoundPlay(e);
}

void BleepoutApp::notifyRoundStarted(RoundState &state) {
  RoundStateEventArgs e(state);
  ofNotifyEvent(roundStartedEvent, e);
  logEvent("RoundStarted", e);
}

void BleepoutApp::notifyRoundPlay(RoundStateEventArgs& e) {
  ofNotifyEvent(roundPlayEvent, e);
  logEvent("RoundPlay", e);
}

void BleepoutApp::notifyRoundEnded(RoundEndedEventArgs& e) {
  ofNotifyEvent(roundEndedEvent, e);
  logEvent("RoundEnded", e);
}

void BleepoutApp::addAnimation(ofPtr<AnimationObject> animation) {
  _animations.push_back(animation);
  auto updater = animation->createUpdaterAction(ofGetElapsedTimef(), _animations);
  addTimedAction(ofPtr<TimedAction>(updater));
}

void BleepoutApp::addTimedAction(ofPtr<TimedAction> action) {
  _timedActions.add(action);
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


#ifdef RADOME

void BleepoutApp::initialize() {
  setup();
}

void BleepoutApp::renderScene(DomeInfo& dome) {
  draw();
}

void BleepoutApp::update(DomeInfo& dome) {
  update();
  _adminController->draw();
}

BleepoutApp app;

#endif
