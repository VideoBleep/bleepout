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
static const int roundQueueLength = 5;

class RoundQueueSlot {
public:
  RoundQueueSlot(ofxUIRadio* radio)
  : _radio(radio) { }
  std::string getSelected() const {
    const auto selected = _radio->getActive();
    if (selected)
      return selected->getName();
    return "";
  }
  bool matchesEvent(const ofxUIEventArgs& e) const {
    for (const auto& toggle : _radio->getToggles()) {
      if (e.widget == toggle)
        return true;
    }
    return false;
  }
private:
  ofxUIRadio* _radio;
};

struct AdminUIControls {
  std::vector<RoundQueueSlot*> roundQueueSlots;
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
  _gui->addWidgetDown(new ofxUILabel("BLEEPOUT ADMIN",
                                     OFX_UI_FONT_LARGE));
  _gui->addSpacer();
  _gui->addWidgetDown(new ofxUILabel("Round", OFX_UI_FONT_MEDIUM));
  _gui->addSpacer();
  
  const auto& allRoundNames = _appParams.queuedRoundNames();
  for (int i = 0; i < roundQueueLength; i++) {
    ofxUIRadio* radio = _gui->addRadio("RoundQueueSlot" + ofToString(i), allRoundNames);
    _controls->roundQueueSlots.push_back(new RoundQueueSlot(radio));
    _gui->addSpacer();
  }
  
  _gui->addRadio("fooo", allRoundNames);
  
  ofAddListener(_gui->newGUIEvent, this,
                &AdminController::onUIEvent);
}

void AdminController::update() {
  _gui->update();
}

void AdminController::draw() {
  _gui->draw();
}

void AdminController::onUIEvent(ofxUIEventArgs &e) {
  for (auto& slot : _controls->roundQueueSlots) {
    if (slot->matchesEvent(e)) {
      ofLogNotice() << "widget event from round slot (id:" << e.widget->getID() << ", name: " << e.widget->getName() << ", selected:" << slot->getSelected() << ")";
      return;
    }
  }
  ofLogNotice() << "OMG widget event from other widget (id:" << e.widget->getID() << ", name: " << e.widget->getName() << ")";
  //...
}
