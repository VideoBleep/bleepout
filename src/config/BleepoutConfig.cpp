//
//  BleepoutConfig.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "BleepoutConfig.h"
#include "BleepoutApp.h"

BleepoutConfig::BleepoutConfig()
: _fps(30),
_logLevel(OF_LOG_NOTICE),
_vsync(true) {
  
}

BleepoutConfig::BleepoutConfig(const BleepoutConfig& other)
: _fps(other._fps)
, _logLevel(other._logLevel)
, _syphonServerName(other._syphonServerName)
, _syphonAppName(other._syphonAppName) { }

BleepoutConfig& BleepoutConfig::operator=(const BleepoutConfig& other) {
  _fps = other._fps;
  _logLevel = other._logLevel;
  _syphonServerName = other._syphonServerName;
  _syphonAppName = other._syphonAppName;
  return *this;
}

BleepoutConfig BleepoutConfig::createTestConfig() {
  BleepoutConfig config;
  config._syphonServerName = "Composition";
  config._syphonAppName = "Arena";
  return config;
}

Json::Value BleepoutConfig::toJsonVal() const {
  Json::Value obj(Json::objectValue);
  obj["fps"] = _fps;
  obj["logLevel"] = (int)_logLevel;
  obj["vsync"] = _vsync;
  obj["syphonServer"] = _syphonServerName;
  obj["syphonApp"] = _syphonAppName;
  return obj;
}

void BleepoutConfig::loadJsonFile(std::string path) {
  Json::Value obj;
  if (!readJsonFile(path, &obj))
    return;
  readJsonVal(obj["fps"], &_fps);
  readJsonEnumVal(obj["logLevel"], &_logLevel);
  readJsonVal(obj["vsync"], &_vsync);
  readJsonVal(obj["syphonServer"], &_syphonServerName);
  readJsonVal(obj["syphonApp"], &_syphonAppName);
}

void BleepoutConfig::saveJsonFile(std::string path) const {
  Json::Value obj = toJsonVal();
  writeJsonFile(path, obj);
}

RoundConfig::RoundConfig(const BleepoutConfig& appConfig)
: _brickSize(7.0f, 5.0f, 17.0f),
_paddleSize(16.0f, 8.0f, 40.0f),
_ballRadius(8.0f),
_brickFadeTime(0.4f),
_domeRadius(150.0f),
_domeMargin(20.0f),
_appConfig(appConfig) { }

void RoundConfig::loadJsonFile(std::string path) {
  Json::Value obj;
  if (!readJsonFile(path, &obj))
    return;
  readJsonVal(obj["brickSize"], &_brickSize);
  readJsonVal(obj["paddleSize"], &_paddleSize);
  readJsonVal(obj["ballRadius"], &_ballRadius);
  readJsonVal(obj["brickFadeTime"], &_brickFadeTime);
  readJsonVal(obj["domeRadius"], &_domeRadius);
  readJsonVal(obj["domeMargin"], &_domeMargin);
  readJsonArr(obj["balls"], &_balls);
  readJsonArr(obj["bricks"], &_bricks);
  readJsonArr(obj["brickRings"], &_brickRings);
  readJsonArr(obj["walls"], &_walls);
  readJsonArr(obj["curvedWallSets"], &_curvedWallSets);
}

Json::Value RoundConfig::toJsonVal() const {
  Json::Value obj;
  obj["brickSize"] = ::toJsonVal(_brickSize);
  obj["paddleSize"] = ::toJsonVal(_paddleSize);
  obj["ballRadius"] = _ballRadius;
  obj["brickFadeTime"] = _brickFadeTime;
  obj["domeRadius"] = _domeRadius;
  obj["domeMargin"] = _domeMargin;
  obj["balls"] = toJsonArr(_balls);
  obj["bricks"] = toJsonArr(_bricks);
  obj["brickRings"] = toJsonArr(_brickRings);
  obj["walls"] = toJsonArr(_walls);
  obj["curvedWallSets"] = toJsonArr(_curvedWallSets);
  return obj;
}

void RoundConfig::saveJsonFile(std::string path) const {
  Json::Value obj = toJsonVal();
  writeJsonFile(path, obj);
}

static void createRingBricks(const BrickRingSpec& ring, std::vector<BrickSpec>& bricks) {
  for (int i = 0; i < ring.count; i++) {
    bricks.push_back(BrickSpec(ring.elevation, i * 360 / (ring.count * 1.0) + ring.phase, ring.color, ring.value, ring.lives, ring.speed));
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
    walls.push_back(WallSpec(theta, phi, ofVec3f(curve.width), curve.isExit, curve.speed));
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
      BrickSpec spec;
      spec.elevation = 30 + 3 * j;
      spec.heading = s * 360 + j * 2 + ((i % 2) ? 5 : -5);
      spec.color = ofColor(s * 255,
                           j / (rows * 1.0) * 255,
                           (1 - s) * 255);
      spec.lives = (j % 3 == 1) ? 2 : 1;
      spec.value = 1;
      spec.speed = 0;
      config.addBrick(spec);
    }
  }
  
  for (int i = 0; i < 6; i++) {
    CurvedWallSpec spec;
    spec.elevation1 = 30;
    spec.heading1 = i * 60 + 15;
    spec.elevation2 = i % 2 ? 70 : 64;
    spec.heading2 = i * 60 + 45;
    spec.width = 10;
    spec.isExit = false;
    spec.speed = 0;
    config.addCurvedWallSet(spec);
  }
    
  config.addWall(WallSpec(67,   5, ofVec3f(10, 10, 30),
                          false, 0.02,  80));
  config.addWall(WallSpec(67, 125, ofVec3f(10, 10, 30),
                          false, 0.02, 200));
  config.addWall(WallSpec(67, 245, ofVec3f(10, 10, 30),
                          false, 0.02, 320));
  
  config.addBrickRing(BrickRingSpec(72, ofColor(0, 0, 0),
                                    12, 1, 2, 0, 0.02));
  config.addBrickRing(BrickRingSpec(76, ofColor(0, 0, 0),
                                    10, 1, 1, 0, -0.02));
  config.addBrickRing(BrickRingSpec(80, ofColor(0, 0, 0),
                                    8, 2, 2, 0, 0.02));

  //...
  return config;
}
