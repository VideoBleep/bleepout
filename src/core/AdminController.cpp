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
#include "SetupController.h"
#include "GameState.h"
#include "BleepoutParameters.h"

static const int uiWidth = 200;

class RoundQueueSlot {
public:
  RoundQueueSlot(ofxUILabelButton* button, int slotIndex)
  : _button(button), _slotIndex(slotIndex) { }
  void setSelectedIndex(int i, const BleepoutConfig& config) {
    _valueIndex = i % config.roundConfigs().size();
    _button->setLabelText(config.roundConfigs()[_valueIndex]->name());
  }
  void setSelectedValue(const std::string& name, const BleepoutConfig& config) {
    for (int i = 0; i < config.roundConfigs().size(); i++) {
      if (config.roundConfigs()[i]->name() == name) {
        _valueIndex = i;
        _button->setLabelText(name);
        return;
      }
    }
  }
  std::string getSelected() const {
    return _button->getLabel()->getLabel();
  }
  bool handleEvent(const ofxUIEventArgs& e,
                   const BleepoutConfig& config) {
    if ((e.widget == _button || e.widget == _button->getLabel()) &&
        _button->getValue()) {
      setSelectedIndex(_valueIndex + 1, config);
      return true;
    }
    return false;
  }
  void updateSlot(BleepoutParameters& params) const {
    params.queuedRoundNames()[_slotIndex] = getSelected();
  }
private:
  ofxUILabelButton* _button;
  int _slotIndex;
  int _valueIndex;
};

struct AdminUIControls {
  std::vector<RoundQueueSlot*> roundQueueSlots;
  ofxUIToggle* timeLimitToggle;
  ofxUINumberDialer* timeLimit;
  ofxUIToggle* pause;
  ofxUILabel* inRound;
  ofxUIToggle* exitsEnabled;
  ofxUIToggle* overrideBallsRespawn;
  ofxUIToggle* ballsRespawn;
  ofxUIToggle* debugGraphics;
  ofxUIToggle* drawTrajectories;
  ofxUIToggle* drawComets;
  ofxUIToggle* drawExtras;
  ofxUIToggle* allLasers;
  ofxUIButton* addBall;
#ifdef ENABLE_SYPHON
  ofxUIToggle* enableSyphon;
  ofxUITextInput* syphonAppName;
  ofxUITextInput* syphonServerName;
#endif
  ofxUIButton* startRound;
  ofxUIButton* endRound;
  ofxUILabel* remainingTime;
  ofxUISlider* audioVolume;
  
  ~AdminUIControls() {
    for (auto& slot : roundQueueSlots)
      delete slot;
  }
  
  void updateQueueSlots(BleepoutParameters& appParams) {
    // going to assume that the number of slots hasn't changed.
    // eventually we should do something better with that..
    auto nameIter = appParams.queuedRoundNames().begin();
    auto slotIter = roundQueueSlots.begin();
    while (nameIter != appParams.queuedRoundNames().end() &&
           slotIter != roundQueueSlots.end()) {
      (*slotIter)->setSelectedValue(*nameIter, appParams.appConfig());
      nameIter++;
      slotIter++;
    }
  }
};

AdminController::AdminController(SetupController& setupController)
: _appConfig(BleepoutParameters::get().appConfig())
, _setupController(setupController)
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
  
  _gui->addLabel("BLEEPOUT ADMIN", OFX_UI_FONT_LARGE);
  _gui->addSpacer();
  _controls->inRound = _gui->addLabel("Not in round", OFX_UI_FONT_MEDIUM);
  _controls->remainingTime = _gui->addLabel("Time: ", OFX_UI_FONT_MEDIUM);
  _gui->addLabel("Round Queue", OFX_UI_FONT_MEDIUM);
  
  const auto& allRoundNames = appParams.queuedRoundNames();
  for (int i = 0; i < appParams.queuedRoundNames().size(); i++) {
    ofxUILabelButton* button = _gui->addLabelButton("RoundQueueSlot" + ofToString(i), true);
    button->setColorBack(ofColor(0, 255, 0));
    button->setColorFill(ofColor(0, 0, 0));
    button->setColorFillHighlight(ofColor(0, 0, 0));
    RoundQueueSlot* slot = new RoundQueueSlot(button, i);
    slot->setSelectedIndex(i, _appConfig);
    _controls->roundQueueSlots.push_back(slot);
  }
  
  _controls->startRound = _gui->addButton("Start Round", false);
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
#ifdef ENABLE_SYPHON
  _controls->enableSyphon = _gui->addLabelToggle("Syphon", &appParams.enableSyphon);
  _controls->syphonAppName = _gui->addTextInput("Syphon App", appParams.syphonAppName);
  _controls->syphonAppName->setTriggerType(OFX_UI_TEXTINPUT_ON_UNFOCUS);
  _controls->syphonServerName = _gui->addTextInput("Syphon Server", appParams.syphonServerName);
  _controls->syphonServerName->setTriggerType(OFX_UI_TEXTINPUT_ON_UNFOCUS);
#endif
  _controls->audioVolume = _gui->addSlider("Audio Volume", 0, 1, &appParams.audioVolume);
  
  ofAddListener(_gui->newGUIEvent, this,
                &AdminController::onUIEvent);
}

void AdminController::attachTo(BleepoutApp &app) {
  ofAddListener(app.roundStartedEvent, this,
                &AdminController::onRoundStarted);
  ofAddListener(app.roundEndedEvent, this,
                &AdminController::onRoundEnded);
}

void AdminController::detachFrom(BleepoutApp &app) {
  ofRemoveListener(app.roundEndedEvent, this,
                   &AdminController::onRoundEnded);
  ofRemoveListener(app.roundStartedEvent, this,
                   &AdminController::onRoundStarted);
}

void AdminController::onRoundStarted(RoundStateEventArgs &e) {
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
}

void AdminController::draw() {
  _gui->draw();
}

static void dumpRoundQueue(BleepoutParameters& params) {
  ofLog log(OF_LOG_NOTICE);
  log << "Round queue:";
  for (const auto& name : params.queuedRoundNames()) {
    log << " " << name;
  }
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
  } else if (e.widget == _controls->endRound &&
             _controls->endRound->getValue()) {
    tryEndRound();
  } else {
    for (auto& slot : _controls->roundQueueSlots) {
      if (slot->handleEvent(e, _appConfig)) {
        slot->updateSlot(appParams);
        dumpRoundQueue(appParams);
        return;
      }
    }
  }
}

bool AdminController::tryStartRound() {
  if (_setupController.canStartRound()) {
    return false;
  }
  auto& appParams = BleepoutParameters::get();
  auto& players = _setupController.lobby();
  std::list<ofPtr<RoundConfig> > rounds = BleepoutParameters::get().getRoundQueue();
  if (notifyTryStartRound(rounds, players)) {
    _controls->updateQueueSlots(appParams);
  }
  return true;
}

bool AdminController::canStartRound() {
  return _setupController.canStartRound();
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

bool AdminController::notifyTryEndRound() {
  EndRoundEventArgs e(END_ADMIN_OVERRIDE);
  ofNotifyEvent(tryEndRoundEvent, e);
  logEvent("TryEndRound", e);
  return e.handled();
}














