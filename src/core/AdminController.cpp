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

static const int uiWidth = 300;

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
  ofxUIToggle* debugGraphics;
  ofxUIToggle* drawTrajectories;
  ofxUIToggle* drawComets;
  ofxUIToggle* drawExtras;
  ofxUIToggle* allLasers;
  ofxUIButton* addBall;
  ofxUIToggle* enableSyphon;
  ofxUITextInput* syphonAppName;
  ofxUITextInput* syphonServerName;
  ofxUIButton* startRound;
  ofxUIButton* endRound;
  
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

AdminController::AdminController(BleepoutParameters& appParams,
                                 SetupController& setupController)
: _appParams(appParams)
, _appConfig(appParams.appConfig())
, _setupController(setupController)
, _gui(NULL), _controls(NULL)
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
  const int totalWidth = ofGetWidth();
  const int totalHeight = ofGetHeight();
  _controls = new AdminUIControls();
  _gui = new ofxUICanvas(totalWidth - uiWidth - 10, 10,
                         uiWidth, totalHeight - 20);
  _gui->setColorBack(ofColor(0, 0, 0, 63));
  
  _gui->addLabel("BLEEPOUT ADMIN", OFX_UI_FONT_LARGE);
  _gui->addSpacer();
  _controls->inRound = _gui->addLabel("Not in round", OFX_UI_FONT_MEDIUM);
  _gui->addLabel("Round Queue", OFX_UI_FONT_MEDIUM);
  
  const auto& allRoundNames = _appParams.queuedRoundNames();
  for (int i = 0; i < _appParams.queuedRoundNames().size(); i++) {
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
  _controls->timeLimitToggle->setValue(_appParams.rules().specifiesTimeLimit());
  
  _controls->timeLimit = _gui->addNumberDialer("Time Limit", 10, 6000, 30, 0);
  _controls->timeLimit->setDisplayLabel(true);
  
  _controls->pause = _gui->addLabelToggle("Pause", &_appParams.paused);
  _controls->exitsEnabled = _gui->addLabelToggle("Exits Enabled", &_appParams.exitsEnabled);
  _controls->debugGraphics = _gui->addLabelToggle("Debug Graphics", &_appParams.debugGraphics);
  _controls->drawTrajectories = _gui->addLabelToggle("Trajectories", &_appParams.drawTrajectories);
  _controls->drawComets = _gui->addLabelToggle("Comets", &_appParams.drawComets);
  _controls->drawExtras = _gui->addLabelToggle("Draw Extras", &_appParams.drawExtras);
  _controls->allLasers = _gui->addLabelToggle("All Lasers", &_appParams.allLasers);
  _controls->addBall = _gui->addButton("Add Ball", false);
  _controls->enableSyphon = _gui->addLabelToggle("Syphon", &_appParams.enableSyphon);
  _controls->syphonAppName = _gui->addTextInput("Syphon App", _appParams.syphonAppName);
  _controls->syphonAppName->setTriggerType(OFX_UI_TEXTINPUT_ON_UNFOCUS);
  _controls->syphonServerName = _gui->addTextInput("Syphon Server", _appParams.syphonServerName);
  _controls->syphonServerName->setTriggerType(OFX_UI_TEXTINPUT_ON_UNFOCUS);
  
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
  _appParams.inRound = true;
}

void AdminController::onRoundEnded(EmptyEventArgs &e) {
  _controls->inRound->setLabel("Not in round");
  _appParams.inRound = false;
}

void AdminController::keyPressed(int key) {
  if (ofGetKeyPressed(BLEEPOUT_CONTROL_KEY)) {
    if (key == 'e') {
      // toggle exits on and off
      _appParams.exitsEnabled = !_appParams.exitsEnabled;
    } else if (key == 't') {
      _appParams.drawTrajectories = !_appParams.drawTrajectories;
    } else if (key == 'd') {
      _appParams.debugGraphics = !_appParams.debugGraphics;
    } else if (key == 'l') {
      _appParams.allLasers = !_appParams.allLasers;
    } else if (key == 'c') {
      _appParams.drawComets= !_appParams.drawComets;
    } else if (key == 'b') {
      // add a new ball
      _appParams.ballsToAdd++;
    }
#ifdef ENABLE_SYPHON
    else if (key == 'y') {
      _appParams.enableSyphon = !_appParams.enableSyphon;
    }
#endif
  }
}

void AdminController::update() {
  _gui->update();
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
  if (e.widget == _controls->syphonAppName) {
    _appParams.syphonAppName = _controls->syphonAppName->getTextString();
  } else if (e.widget == _controls->syphonServerName) {
    _appParams.syphonServerName = _controls->syphonServerName->getTextString();
  } else if (e.widget == _controls->enableSyphon) {
    _appParams.enableSyphon = _controls->enableSyphon->getValue();
  } else if (e.widget == _controls->addBall && _controls->addBall->getValue()) {
    _appParams.ballsToAdd++;
  } else if (e.widget == _controls->timeLimitToggle ||
      e.widget == _controls->timeLimit) {
    if (_controls->timeLimitToggle->getValue()) {
      _appParams.rules().setTimeLimit(_controls->timeLimit->getValue());
    } else {
      _appParams.rules().unsetTimeLimit();
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
        slot->updateSlot(_appParams);
        ofLogNotice() << "widget event from round slot (id:" << e.widget->getID() << ", name: " << e.widget->getName() << ", selected:" << slot->getSelected() << ")";
        dumpRoundQueue(_appParams);
        return;
      }
    }
    ofLogNotice() << "OMG widget event from other widget (id:" << e.widget->getID() << ", name: " << e.widget->getName() << ")";
  }
}

bool AdminController::tryStartRound() {
  auto& players = _setupController._players;
  ofPtr<RoundConfig> roundConfig = _appParams.getNextRound();
  if (!roundConfig)
    return false;
  if (notifyTryStartRound(roundConfig, players)) {
    _appParams.popNextRound();
    _controls->updateQueueSlots(_appParams);
  }
}

bool AdminController::canStartRound() {
  return _setupController.canStartRound();
}

void AdminController::tryEndRound() {
  notifyTryEndRound();
}
bool AdminController::notifyTryStartRound(ofPtr<RoundConfig> config,
                      std::list<ofPtr<Player> > players) {
  StartRoundEventArgs e(config, players);
  ofNotifyEvent(tryStartRoundEvent, e);
  logEvent("TryStartRound", e);
  return e.handled();
}

bool AdminController::notifyTryEndRound() {
  EndRoundEventArgs e;
  ofNotifyEvent(tryEndRoundEvent, e);
  logEvent("TryEndRound", e);
  return e.handled();
}














