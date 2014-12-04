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
  ofxXmlSettings settings(path);
  _fps = settings.getValue("settings:fps", 30);
  _logLevel = (ofLogLevel)settings.getValue("settings:logLevel", OF_LOG_NOTICE);
  _vsync = settings.getValue("settings:vsync", true);
  //...
}

void BleepoutConfig::saveFile(const std::string& path) const {
  ofxXmlSettings settings;
  settings.setValue("settings:fps", _fps);
  settings.setValue("settings:logLevel", (int)_logLevel);
  settings.setValue("settings:vsync", _vsync);
  //...
  settings.save(path);
}

void RoundConfig::loadFile(const std::string &path) {
  ofxXmlSettings settings(path);
  _brickSize.x = settings.getValue("settings:brickSizeX", 100.0f);
  _brickSize.y = settings.getValue("settings:brickSizeY", 20.0f);
  _brickGap = settings.getValue("settings:brickGap", 5.0f);
  _paddleSize.x = settings.getValue("settings:paddleSizeX", 150.0f);
  _paddleSize.y = settings.getValue("settings:paddleSizeY", 25.0f);
  _ballRadius = settings.getValue("settings:ballRadius", 10.0f);
  _ballDensity = settings.getValue("settings:ballDensity", 3.0f);
  _ballBounce = settings.getValue("settings:ballBounce", 1.0f);
  _ballFriction = settings.getValue("settings:ballFriction", 0.0f);
  _paddleDensity = settings.getValue("settings:paddleDensity", 30.0f);
  _paddleBounce = settings.getValue("settings:paddleBounce", 0.5f);
  _paddleFriction = settings.getValue("settings:paddleFriction", 0.9f);
  //...
}

void RoundConfig::saveFile(const std::string &path) const {
  ofxXmlSettings settings;
  settings.setValue("settings:brickSizeX", _brickSize.x);
  settings.setValue("settings:brickSizeY", _brickSize.y);
  settings.setValue("settings:brickGap", _brickGap);
  settings.setValue("settings:paddleSizeX", _paddleSize.x);
  settings.setValue("settings:paddleSizeY", _paddleSize.y);
  settings.setValue("settings:ballRadius", _ballRadius);
  settings.setValue("settings:ballDensity", _ballDensity);
  settings.setValue("settings:ballBounce", _ballBounce);
  settings.setValue("settings:ballFriction", _ballFriction);
  settings.setValue("settings:paddleDensity", _paddleDensity);
  settings.setValue("settings:paddleBounce", _paddleBounce);
  settings.setValue("settings:paddleFriction", _paddleFriction);

  //...
  settings.save(path);

}

