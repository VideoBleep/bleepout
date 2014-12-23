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

static const int uiWidth = 300;
static const int uiHeight = 500;

AdminController::AdminController(BleepoutConfig& appConfig)
: _appConfig(appConfig), _gui(NULL) { }

AdminController::~AdminController() {
  if (_gui)
    delete _gui;
}

void AdminController::setup() {
  const int totalWidth = ofGetWidth();
  const int totalHeight = ofGetHeight();
  _gui = new ofxUICanvas(totalWidth - uiWidth - 10, 10,
                         uiWidth, uiHeight);
  _gui->addWidgetDown(new ofxUILabel("BLEEPOUT ADMIN",
                                     OFX_UI_FONT_LARGE));
  _gui->addSpacer();
  _gui->addWidgetDown(new ofxUILabel("Round", OFX_UI_FONT_MEDIUM));
  _gui->addSpacer();
  std::vector<std::string> roundNames;
  for (const auto& round : _appConfig.roundConfigs())
    roundNames.push_back(round->name());
  _gui->addDropDownList("RoundList", roundNames);
}

void AdminController::update() {
  _gui->update();
}

void AdminController::draw() {
  _gui->draw();
}

void AdminController::onUIEvent(ofxUIEventArgs &e) {
  //...
}
