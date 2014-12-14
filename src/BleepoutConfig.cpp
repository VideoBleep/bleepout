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
: _brickSize(7.0f, 5.0f, 17.0f),
_paddleSize(16.0f, 8.0f, 40.0f),
_ballRadius(8.0f),
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
  readJsonVal(root, "domeRadius", &_domeRadius);
  readJsonVal(root, "domeMargin", &_domeMargin);
}

void RoundConfig::saveJsonFile(std::string path) const {
  Json::Value root;
  root["brickSize"] = toJsonObj(_brickSize);
  root["paddleSize"] = toJsonObj(_paddleSize);
  root["ballRadius"] = _ballRadius;
  root["domeRadius"] = _domeRadius;
  root["domeMargin"] = _domeMargin;
  writeJsonFile(path, root);
}

static void createRingBricks(const BrickRingSpec& ring, std::vector<BrickSpec>& bricks) {
  for (int i = 0; i < ring.count; i++) {
    bricks.push_back(BrickSpec(ring.elevation, i * 360 / (ring.count * 1.0) + ring.phase, ring.color));
  }
}

std::vector<BrickSpec> RoundConfig::allBricks() const {
  std::vector<BrickSpec> allBricks(_bricks);
  for (const BrickRingSpec& ring : _brickRings) {
    createRingBricks(ring, allBricks);
  }
  return allBricks;
}

static void createCurveWalls(const CurvedWallSpec& curve, float r, std::vector<WallSpec>& walls) {
  float theta = curve.elevation1;
  float phi = curve.heading1;
  float dtheta = curve.elevation2 - curve.elevation1;
  float dphi = curve.heading2 - curve.heading1;
  int steps = floor(max((r * dtheta * PI/180.0) / curve.width, (r * dphi * PI/180.0) / curve.width));
  dtheta /= steps * 1.0;
  dphi /= steps * 1.0;
  for (int i = 0; i < steps; i++) {
    walls.push_back(WallSpec(theta, phi, ofVec3f(curve.width)));
    theta += dtheta;
    phi += dphi;
  }
}

std::vector<WallSpec> RoundConfig::allWalls() const {
  std::vector<WallSpec> walls(_walls);
  float r = domeRadius() + domeMargin();
  for (const CurvedWallSpec& curve : _curvedWallSets) {
    createCurveWalls(curve, r, walls);
  }
  return walls;
}

RoundConfig RoundConfig::createTestConfig(const BleepoutConfig &appConfig) {
  RoundConfig config(appConfig);
  
  for (int i = 0; i < 5; i ++) {
    config.addBall(BallSpec(30, ofRandom(360)));
  }
  
  int cols = 12;
  int rows = 10;
  
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++) {
      float s = i / (cols * 1.0);
      config.addBrick(BrickSpec(30 + 3 * j,
                           s * 360 + j * 2 + ((i % 2) ? 5 : -5),
                           ofColor(s * 255, j / (rows * 1.0) * 255, (1 - s) * 255)));
    }
  }
  
  for (int i = 0; i < 6; i++) {
    config.addCurvedWallSet(CurvedWallSpec(30, i * 60 + 15, 70, i * 60 + 45, 10));
  }
  
  config.addBrickRing(BrickRingSpec(72, ofColor(0, 0, 0), 12));
  config.addBrickRing(BrickRingSpec(76, ofColor(0, 0, 0), 10));
  config.addBrickRing(BrickRingSpec(80, ofColor(0, 0, 0), 8));
  //...
  return config;
}
