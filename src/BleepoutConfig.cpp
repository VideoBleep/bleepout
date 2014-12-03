//
//  BleepoutConfig.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "BleepoutConfig.h"
#include <ofxXmlSettings.h>

void BleepoutConfig::loadFile(const std::string& path) {
  ofxXmlSettings settings;
  settings.load(path);
  //...
}

void BleepoutConfig::saveFile(const std::string& path) {
  ofxXmlSettings settings;
  //...
  settings.save(path);
}
