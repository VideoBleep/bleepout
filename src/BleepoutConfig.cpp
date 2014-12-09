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
#include <json.h> // it's included as part of ofxLibwebsockets
#include <fstream>

BleepoutConfig::BleepoutConfig()
: _fps(30),
_logLevel(OF_LOG_NOTICE),
_vsync(true) {
  
}

void BleepoutConfig::loadFile(const std::string& path) {
  ofxXmlSettings settings;
  if (!path.empty())
    settings.load(path);
  _fps = settings.getValue("settings:fps", _fps);
  _logLevel = (ofLogLevel)settings.getValue("settings:logLevel", _logLevel);
  _vsync = settings.getValue("settings:vsync", _vsync);
  //...
}

void BleepoutConfig::saveFile(const std::string& path) const {
  ofxXmlSettings settings;
  settings.setValue("settings:fps", _fps);
  settings.setValue("settings:logLevel", (int)_logLevel);
  settings.setValue("settings:vsync", _vsync);
  settings.save(path);
}

void BleepoutConfig::saveJsonFile(std::string path) const {
  path = ofToDataPath(path);
  std::ifstream fis(path.c_str());
  Json::Reader reader;
  Json::Value root;
  if (!reader.parse(fis, root)) {
    ofLogError() << "error loading json from: " << path << ": " << reader.getFormattedErrorMessages();
    //...??????
    return;
  }
  
  //...
}

static void readPhysics(ofxXmlSettings& settings,
                                  std::string prefix,
                                  PhysicsOptions& vals) {
  vals.density = settings.getValue(prefix + "Density", vals.density);
  vals.bounce = settings.getValue(prefix + "Bounce", vals.bounce);
  vals.friction = settings.getValue(prefix + "Friction", vals.friction);
}

static void writePhysics(ofxXmlSettings& settings,
                         std::string prefix,
                         const PhysicsOptions& options) {
  settings.setValue(prefix + "Density", options.density);
  settings.setValue(prefix + "Bounce", options.bounce);
  settings.setValue(prefix + "Friction", options.friction);
}

static void readVec2f(ofxXmlSettings& settings, std::string prefix, ofVec2f& vals) {
  vals.x = settings.getValue(prefix + "X", vals.x);
  vals.y = settings.getValue(prefix + "Y", vals.y);
}

static void writeVec2(ofxXmlSettings& settings, std::string prefix, ofVec2f vals) {
  settings.setValue(prefix + "X", vals.x);
  settings.setValue(prefix + "Y", vals.y);
}

RoundConfig::RoundConfig(const BleepoutConfig& appConfig)
: _brickSize(100.0f, 20.0f),
_brickGap(5.0f),
_paddleSize(150.0f, 25.0f),
_ballRadius(10.0f),
_ballPhysics(3.0f, 1.0f, 0.0f),
_paddlePhysics(0.0f, 0.0f, 0.9f),
_ballInitialVelocity(0.01f, 10.5f),
_appConfig(appConfig) { }

void RoundConfig::loadFile(const std::string &path) {
  ofxXmlSettings settings;
  if (!path.empty())
    settings.load(path);
  readVec2f(settings, "settings:brickSize", _brickSize);
  _brickGap = settings.getValue("settings:brickGap", _brickGap);
  readVec2f(settings, "settings:paddleSize", _paddleSize);
  _ballRadius = settings.getValue("settings:ballRadius", _ballRadius);
  readPhysics(settings, "settings:ball", _ballPhysics);
  readPhysics(settings, "settings:paddle", _paddlePhysics);
  readVec2f(settings, "settings:ballInitialVelocity", _ballInitialVelocity);
  //...
}

void RoundConfig::saveFile(const std::string &path) const {
  ofxXmlSettings settings;
  writeVec2(settings, "settings:brickSize", _brickSize);
  settings.setValue("settings:brickGap", _brickGap);
  writeVec2(settings, "settings:paddleSize", _paddleSize);
  settings.setValue("settings:ballRadius", _ballRadius);
  writePhysics(settings, "settings:ball", _ballPhysics);
  writePhysics(settings, "settings:paddle", _paddlePhysics);
  writeVec2(settings, "settings:ballInitialVelocity", _ballInitialVelocity);

  //...
  settings.save(path);
}

