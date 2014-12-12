//
//  BleepoutConfig.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "BleepoutConfig.h"
#include "BleepoutApp.h"
#include <json.h> // it's included as part of ofxLibwebsockets
#include <fstream>
#include "JsonUtil.h"

BleepoutConfig::BleepoutConfig()
: _fps(30),
_logLevel(OF_LOG_NOTICE),
_vsync(true) {
  
}

void BleepoutConfig::loadJsonFile(std::string path) {
  Json::Value root;
  if (!readJsonFile(path, &root))
    return;
  readJsonVal(root, "fps", &_fps);
  readJsonEnumVal(root, "logLevel", &_logLevel);
  readJsonVal(root, "vsync", &_vsync);
}

void BleepoutConfig::saveJsonFile(std::string path) const {
  Json::Value root(Json::objectValue);
  root["fps"] = _fps;
  root["logLevel"] = (int)_logLevel;
  root["vsync"] = _vsync;
  writeJsonFile(path, root);
}

RoundConfig::RoundConfig(const BleepoutConfig& appConfig)
: _brickSize(20.0f, 5.0f, 5.0f),
_paddleSize(16.0f, 8.0f, 40.0f),
_ballRadius(8.0f),
_ballInitialVelocity(0.0f, 30.5f, 0.0f),
_domeRadius(150.0f),
_domeMargin(20.0f),
_appConfig(appConfig) { }

void RoundConfig::loadJsonFile(std::string path) {
  Json::Value root;
  if (!readJsonFile(path, &root))
    return;
  readJsonVal(root, "brickSize", &_brickSize);
  readJsonVal(root, "paddleSize", &_paddleSize);
  readJsonVal(root, "ballRadius", &_ballRadius);
  readJsonVal(root, "ballInitialVelocity", &_ballInitialVelocity);
  readJsonVal(root, "domeRadius", &_domeRadius);
  readJsonVal(root, "domeMargin", &_domeMargin);
  readJsonVal(root, "playerColors", &_playerColors);
}

void RoundConfig::saveJsonFile(std::string path) const {
  Json::Value root;
  root["brickSize"] = toJsonObj(_brickSize);
  root["paddleSize"] = toJsonObj(_paddleSize);
  root["ballRadius"] = _ballRadius;
  root["ballInitialVelocity"] = toJsonObj(_ballInitialVelocity);
  root["domeRadius"] = _domeRadius;
  root["domeMargin"] = _domeMargin;
  root["playerColors"] = toJsonArr(_playerColors);
  writeJsonFile(path, root);
}

