//
//  BleepoutConfig.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "BleepoutConfig.h"
#include "BleepoutApp.h"
#include <ofxXmlSettings.h>

/*static*/ const BleepoutConfig& BleepoutConfig::getInstance() {
  auto app = BleepoutApp::getApp();
  return app->config();
}

void BleepoutConfig::loadFile(const std::string& path) {
  ofxXmlSettings settings;
  settings.load(path);
  _fps = settings.getValue("settings:fps", 30);
  //...
}

void BleepoutConfig::saveFile(const std::string& path) {
  ofxXmlSettings settings;
  settings.setValue("settings:fps", _fps);
  //...
  settings.save(path);
}
