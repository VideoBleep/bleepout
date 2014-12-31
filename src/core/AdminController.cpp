//
//  AdminController.cpp
//  bleepout
//
//  Created by tekt on 12/22/14.
//
//

#include "AdminController.h"
#include <vector>
#include <string>
#include <ofMain.h>
#include "GameEvents.h"
#include "BleepoutApp.h"
#include "GameState.h"
#include "BleepoutParameters.h"

static const int uiWidth = 200;

struct AdminUIControls {
  ofxUIButton* reloadConfig;
  ofxUIToggle* timeLimitToggle;
  ofxUINumberDialer* timeLimit;
  ofxUIToggle* pause;
  ofxUILabel* inRound;
  ofxUILabel* lobbyCount;
  ofxUILabel* roundPlayersCount;
  ofxUIToggle* exitsEnabled;
  ofxUIToggle* overrideBallsRespawn;
  ofxUIToggle* ballsRespawn;
  ofxUIToggle* debugGraphics;
  ofxUIToggle* drawTrajectories;
  ofxUIToggle* drawComets;
  ofxUIToggle* drawExtras;
  ofxUIToggle* allLasers;
  ofxUIButton* addBall;
  ofxUIButton* addManyBalls;
#ifdef ENABLE_SYPHON
  ofxUIToggle* enableSyphon;
  ofxUITextInput* syphonAppName;
  ofxUITextInput* syphonServerName;
#endif
  ofxUIButton* startRound;
  ofxUIButton* playRound;
  ofxUIButton* endRound;
  ofxUILabel* remainingTime;
  ofxUISlider* audioVolume;
};

AdminController::AdminController()
: _appConfig(BleepoutParameters::get().appConfig())
, _gui(NULL), _controls(NULL), _roundState(NULL)
, EventSource() { }

AdminController::~AdminController() {
  if (_controls) {
    delete _controls;
  }
  if (_gui) {
    ofRemoveListener(_gui->newGUIEvent, this,
                     &AdminController::onUIEvent);
    delete _gui;
  }
}

void AdminController::setup() {
  auto& appParams = BleepoutParameters::get();
  const int totalWidth = ofGetWidth();
  const int totalHeight = ofGetHeight();
  _controls = new AdminUIControls();
  _gui = new ofxUICanvas(totalWidth - uiWidth - 10, 0,
                         uiWidth, totalHeight);
  _gui->setColorBack(ofColor(0, 0, 0, 63));
  _gui->setFont("GUI/Exo-Regular.ttf", true, true, false, 0.0, OFX_UI_FONT_RESOLUTION);
  _gui->addLabel("BLEEPOUT", OFX_UI_FONT_LARGE);
  _gui->addSpacer();
  _controls->inRound = _gui->addLabel("Not in round", OFX_UI_FONT_MEDIUM);
  _controls->remainingTime = _gui->addLabel("Time: ", OFX_UI_FONT_MEDIUM);
  _controls->lobbyCount = _gui->addLabel("Lobby players: 0");
  _controls->roundPlayersCount = _gui->addLabel("Round players: 0");
  
  _controls->startRound = _gui->addButton("Start Round", false);
  _controls->playRound = _gui->addButton("Play Round", false);
  _controls->endRound = _gui->addButton("End Round", false);
  
  _gui->addSpacer();
  
  _controls->timeLimitToggle = _gui->addToggle("Time Limit Enabled", false);
  _controls->timeLimitToggle->setLabelVisible(true);
  _controls->timeLimitToggle->setValue(appParams.rules().specifiesTimeLimit());
  
  _controls->timeLimit = _gui->addNumberDialer("Time Limit", 1, 6000, 30, 0);
  _controls->timeLimit->setDisplayLabel(true);
  
  _controls->pause = _gui->addLabelToggle("Pause", &appParams.paused);
  _controls->exitsEnabled = _gui->addLabelToggle("Exits Enabled", &appParams.exitsEnabled);
  
  _controls->overrideBallsRespawn = _gui->addToggle("Override Respawn", false);
  _controls->overrideBallsRespawn->setLabelVisible(true);
  _controls->overrideBallsRespawn->setValue(false);
  _controls->ballsRespawn = _gui->addLabelToggle("Balls Respawn", appParams.rules().ballsRespawn());
  
  _controls->debugGraphics = _gui->addLabelToggle("Debug Graphics", &appParams.debugGraphics);
  _controls->drawTrajectories = _gui->addLabelToggle("Trajectories", &appParams.drawTrajectories);
  _controls->drawComets = _gui->addLabelToggle("Comets", &appParams.drawComets);
  _controls->drawExtras = _gui->addLabelToggle("Draw Extras", &appParams.drawExtras);
  _controls->allLasers = _gui->addLabelToggle("All Lasers", &appParams.allLasers);
  _controls->addBall = _gui->addButton("Add Ball", false);
  _controls->addManyBalls = _gui->addButton("Add Many Balls", false);
#ifdef ENABLE_SYPHON
  _controls->enableSyphon = _gui->addLabelToggle("Syphon", &appParams.enableSyphon);
  _controls->syphonAppName = _gui->addTextInput("Syphon App", appParams.syphonAppName);
  _controls->syphonAppName->setTriggerType(OFX_UI_TEXTINPUT_ON_UNFOCUS);
  _controls->syphonServerName = _gui->addTextInput("Syphon Server", appParams.syphonServerName);
  _controls->syphonServerName->setTriggerType(OFX_UI_TEXTINPUT_ON_UNFOCUS);
#endif
  _controls->audioVolume = _gui->addSlider("Audio Volume", 0, 1, &appParams.audioVolume);
  
  _controls->reloadConfig = _gui->addButton("Reload Config", false);
  
  ofAddListener(_gui->newGUIEvent, this,
                &AdminController::onUIEvent);
}

void AdminController::attachTo(BleepoutApp &app) {
  ofAddListener(app.roundStartedEvent, this,
                &AdminController::onRoundStarted);
  ofAddListener(app.roundPlayEvent, this,
                &AdminController::onRoundPlay);
  ofAddListener(app.roundEndedEvent, this,
                &AdminController::onRoundEnded);
}

void AdminController::detachFrom(BleepoutApp &app) {
  ofRemoveListener(app.roundEndedEvent, this,
                   &AdminController::onRoundEnded);
  ofRemoveListener(app.roundPlayEvent, this,
                   &AdminController::onRoundPlay);
  ofRemoveListener(app.roundStartedEvent, this,
                   &AdminController::onRoundStarted);
}

void AdminController::onRoundStarted(RoundStateEventArgs &e) {
  _controls->inRound->setLabel("Sort of in round...");
  BleepoutParameters::get().inRound = true;
  _roundState = &e.state();
}

void AdminController::onRoundPlay(RoundStateEventArgs &e) {
  _controls->inRound->setLabel("In round: " + e.state().config().name());
  BleepoutParameters::get().inRound = true;
  _roundState = &e.state();
}

void AdminController::onRoundEnded(RoundEndedEventArgs &e) {
  _controls->inRound->setLabel("Not in round");
  BleepoutParameters::get().inRound = false;
  _roundState = NULL;
}

void AdminController::keyPressed(int key) {
  auto& appParams = BleepoutParameters::get();
  if (ofGetKeyPressed(BLEEPOUT_CONTROL_KEY)) {
    if (key == 'e') {
      // toggle exits on and off
      appParams.exitsEnabled = !appParams.exitsEnabled;
    } else if (key == 't') {
      appParams.drawTrajectories = !appParams.drawTrajectories;
    } else if (key == 'd') {
      appParams.debugGraphics = !appParams.debugGraphics;
    } else if (key == 'l') {
      appParams.allLasers = !appParams.allLasers;
    } else if (key == 'c') {
      appParams.drawComets= !appParams.drawComets;
    } else if (key == 'b') {
      // add a new ball
      appParams.ballsToAdd++;
    }
#ifdef ENABLE_SYPHON
    else if (key == 'y') {
      appParams.enableSyphon = !appParams.enableSyphon;
    }
#endif
  }
}

void AdminController::update() {
  std::string timeText = "Time: ";
  if (_roundState && _roundState->endTime > 0)
    timeText += ofToString(_roundState->remainingTime());
  _controls->remainingTime->setLabel(timeText);
  _gui->update();
  _gui->setPosition(ofGetWidth() - uiWidth - 10, 0);
  _gui->setHeight(ofGetHeight());
  _controls->lobbyCount->setLabel("Lobby players: " + ofToString(_lobby.size()));
  std::string rplayersText = "Round players: ";
  if (_roundState) {
    rplayersText += ofToString(_roundState->players().size());
  } else {
    rplayersText += "-";
  }
  _controls->roundPlayersCount->setLabel(rplayersText);
}

void AdminController::draw() {
  _gui->draw();
}

void AdminController::onUIEvent(ofxUIEventArgs &e) {
  auto& appParams = BleepoutParameters::get();
#ifdef ENABLE_SYPHON
  if (e.widget == _controls->syphonAppName) {
    appParams.syphonAppName = _controls->syphonAppName->getTextString();
  } else if (e.widget == _controls->syphonServerName) {
    appParams.syphonServerName = _controls->syphonServerName->getTextString();
  } else if (e.widget == _controls->enableSyphon) {
    appParams.enableSyphon = _controls->enableSyphon->getValue();
  }
#endif
  if (e.widget == _controls->addBall && _controls->addBall->getValue()) {
    appParams.ballsToAdd++;
  } else if (e.widget == _controls->addManyBalls && _controls->addManyBalls->getValue()) {
    appParams.ballsToAdd += 20;
  } else if (e.widget == _controls->timeLimitToggle ||
             e.widget == _controls->timeLimit) {
    if (_controls->timeLimitToggle->getValue()) {
      appParams.rules().setTimeLimit(_controls->timeLimit->getValue());
    } else {
      appParams.rules().unsetTimeLimit();
    }
  } else if (e.widget == _controls->overrideBallsRespawn ||
             e.widget == _controls->ballsRespawn) {
    if (_controls->overrideBallsRespawn->getValue()) {
      appParams.rules().setBallsRespawn(_controls->ballsRespawn->getValue());
    } else {
      appParams.rules().unsetBallsRespawn();
    }
  } else if (e.widget == _controls->startRound &&
             _controls->startRound->getValue()) {
    tryStartRound();
  } else if (e.widget == _controls->playRound &&
             _controls->playRound->getValue()) {
    notifyPlayRound();
  } else if (e.widget == _controls->endRound &&
             _controls->endRound->getValue()) {
    tryEndRound();
  } else if (e.widget == _controls->reloadConfig &&
             _controls->reloadConfig->getValue()) {
    reloadConfig();
  }
}

bool AdminController::tryStartRound() {
  if (!canStartRound()) {
    return false;
  }
  auto& appParams = BleepoutParameters::get();
  auto& players = _lobby;
  std::list<ofPtr<RoundConfig> > rounds = BleepoutParameters::get().getRoundQueue();
  if (notifyTryStartRound(rounds, players)) {
    _lobby.clear();
  }
  return true;
}

bool AdminController::canStartRound() {
  return !_lobby.empty();
}

void AdminController::tryEndRound() {
  notifyTryEndRound();
}
bool AdminController::notifyTryStartRound(std::list<ofPtr<RoundConfig> > configs,
                                          std::list<ofPtr<Player> > players) {
  StartRoundEventArgs e(configs, players);
  ofNotifyEvent(tryStartRoundEvent, e);
  logEvent("TryStartRound", e);
  return e.handled();
}

void AdminController::notifyPlayRound() {
  EmptyEventArgs e;
  ofNotifyEvent(playRoundEvent, e);
  logEvent("PlayRound", e);
}

bool AdminController::notifyTryEndRound() {
  EndRoundEventArgs e(END_ADMIN_OVERRIDE);
  ofNotifyEvent(tryEndRoundEvent, e);
  logEvent("TryEndRound", e);
  return e.handled();
}

void AdminController::handlePlayerConnected(PlayerEventArgs& e) {
  _lobby.push_back(ofPtr<Player>(e.player()));
}

void AdminController::reloadConfig() {
  _appConfig.loadJsonFile("config/bleepoutConfig.json");
}













