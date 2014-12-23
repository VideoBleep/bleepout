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

struct AdminUIControls {
  std::vector<ofxUIDropDownList*> roundQueueSlots;
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
  if (_controls)
    delete _controls;
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
    ofxUIDropDownList* slot = new ofxUIDropDownList("RoundQueueSlot" + ofToString(i), allRoundNames);
    _gui->addWidgetDown(slot);
    _controls->roundQueueSlots.push_back(slot);
    _gui->addSpacer();
  }
  
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
    if (slot == e.widget) {
      ofLogNotice() << "widget event from round slot (id:" << e.widget->getID() << ", name: " << e.widget->getName() << ")";
      return;
    }
  }
  ofLogNotice() << "OMG widget event from other widget (id:" << e.widget->getID() << ", name: " << e.widget->getName() << ")";
  //...
}
