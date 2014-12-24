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

static const int uiWidth = 300;
static const int uiHeight = 500;

class RoundQueueSlot {
public:
  RoundQueueSlot(ofxUILabelButton* button, int slotIndex)
  : _button(button), _slotIndex(slotIndex) { }
  void setSelectedIndex(int i, const BleepoutConfig& config) {
    _valueIndex = i % config.roundConfigs().size();
    _button->setLabelText(config.roundConfigs()[_valueIndex]->name());
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
};

AdminController::AdminController(BleepoutParameters& appParams)
: _appParams(appParams)
, _appConfig(appParams.appConfig())
, _gui(NULL), _controls(NULL) { }

AdminController::~AdminController() {
  if (_gui) {
    ofRemoveListener(_gui->newGUIEvent, this,
                     &AdminController::onUIEvent);
    delete _gui;
  }
  if (_controls) {
    for (auto slot : _controls->roundQueueSlots) {
      delete slot;
    }
    delete _controls;
  }
}

void AdminController::setup() {
  const int totalWidth = ofGetWidth();
  const int totalHeight = ofGetHeight();
  _controls = new AdminUIControls();
  _gui = new ofxUICanvas(totalWidth - uiWidth - 10, 10,
                         uiWidth, uiHeight);
  _gui->setColorBack(ofColor(0, 0, 0, 63));
  
  _gui->addWidgetDown(new ofxUILabel("BLEEPOUT ADMIN",
                                     OFX_UI_FONT_LARGE));
  _gui->addSpacer();
  _gui->addWidgetDown(new ofxUILabel("Round Queue",
                                     OFX_UI_FONT_MEDIUM));
  _gui->addSpacer();
  
  const auto& allRoundNames = _appParams.queuedRoundNames();
  for (int i = 0; i < _appParams.queuedRoundNames().size(); i++) {
    ofxUILabelButton* button = _gui->addLabelButton("RoundQueueSlot" + ofToString(i), true);
    button->setColorBack(ofColor(0, 255, 0));
    button->setColorFill(ofColor(0, 0, 0));
    button->setColorFillHighlight(ofColor(0, 0, 0));
    RoundQueueSlot* slot = new RoundQueueSlot(button, i);
    slot->setSelectedIndex(i, _appConfig);
    _controls->roundQueueSlots.push_back(slot);
    
    _gui->addSpacer();
  }
  
  _controls->timeLimitToggle = _gui->addToggle("Time Limit Enabled", false);
  _controls->timeLimitToggle->setLabelVisible(true);
  _controls->timeLimitToggle->setValue(_appParams.rules().specifiesTimeLimit());
  
  _controls->timeLimit = _gui->addNumberDialer("Time Limit", 10, 6000, 30, 0);
  _controls->timeLimit->setDisplayLabel(true);
  
  ofAddListener(_gui->newGUIEvent, this,
                &AdminController::onUIEvent);
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
  for (auto& slot : _controls->roundQueueSlots) {
    if (slot->handleEvent(e, _appConfig)) {
      slot->updateSlot(_appParams);
      ofLogNotice() << "widget event from round slot (id:" << e.widget->getID() << ", name: " << e.widget->getName() << ", selected:" << slot->getSelected() << ")";
      dumpRoundQueue(_appParams);
      return;
    }
  }
  if (e.widget == _controls->timeLimitToggle ||
      e.widget == _controls->timeLimit) {
    if (_controls->timeLimitToggle->getValue()) {
      _appParams.rules().setTimeLimit(_controls->timeLimit->getValue());
    } else {
      _appParams.rules().unsetTimeLimit();
    }
    return;
  }
  ofLogNotice() << "OMG widget event from other widget (id:" << e.widget->getID() << ", name: " << e.widget->getName() << ")";
  //...
}
