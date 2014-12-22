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
_vsync(true) { }

BleepoutConfig* BleepoutConfig::createConfig() {
  BleepoutConfig* config = new BleepoutConfig();
  config->_syphonServerName = "Composition";
  config->_syphonAppName = "Arena";
  config->_roundConfigs.push_back(ofPtr<RoundConfig>(RoundConfig::createRoundConfig1()));
  config->_roundConfigs.push_back(ofPtr<RoundConfig>(RoundConfig::createRoundConfig2()));
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

RoundConfig::RoundConfig(std::string name)
: _brickSize(7.0f, 5.0f, 17.0f),
_paddleSize(16.0f, 8.0f, 40.0f),
_ballRadius(8.0f),
_modifierRadius(9.0f),
_brickFadeTime(0.4f),
_modifierFadeTime(0.2f),
_domeRadius(150.0f),
_domeMargin(20.0f),
_name(name),
_startDelay(0) { }

void RoundConfig::loadJsonFile(std::string path) {
  Json::Value obj;
  if (!readJsonFile(path, &obj))
    return;
  readJsonVal(obj["brickSize"], &_brickSize);
  readJsonVal(obj["paddleSize"], &_paddleSize);
  readJsonVal(obj["ballRadius"], &_ballRadius);
  readJsonVal(obj["modifierRadius"], &_modifierRadius);
  readJsonVal(obj["brickFadeTime"], &_brickFadeTime);
  readJsonVal(obj["domeRadius"], &_domeRadius);
  readJsonVal(obj["domeMargin"], &_domeMargin);
  readJsonArr(obj["balls"], &_balls);
  readJsonArr(obj["bricks"], &_bricks);
  readJsonArr(obj["brickRings"], &_brickRings);
  readJsonArr(obj["walls"], &_walls);
  readJsonArr(obj["curvedWallSets"], &_curvedWallSets);
  readJsonVal(obj["modifierDefs"], &_modifierDefs);
}

Json::Value RoundConfig::toJsonVal() const {
  Json::Value obj;
  obj["brickSize"] = ::toJsonVal(_brickSize);
  obj["paddleSize"] = ::toJsonVal(_paddleSize);
  obj["ballRadius"] = _ballRadius;
  obj["modifierRadius"] = _modifierRadius;
  obj["brickFadeTime"] = _brickFadeTime;
  obj["domeRadius"] = _domeRadius;
  obj["domeMargin"] = _domeMargin;
  obj["balls"] = toJsonArr(_balls);
  obj["bricks"] = toJsonArr(_bricks);
  obj["brickRings"] = toJsonArr(_brickRings);
  obj["walls"] = toJsonArr(_walls);
  obj["curvedWallSets"] = toJsonArr(_curvedWallSets);
  obj["modifierDefs"] = ::toJsonVal(_modifierDefs);
  return obj;
}

void RoundConfig::saveJsonFile(std::string path) const {
  Json::Value obj = toJsonVal();
  writeJsonFile(path, obj);
}

static void createRingBricks(const BrickRingSpec& ring, std::vector<BrickSpec>& bricks) {
  for (int i = 0; i < ring.count; i++) {
    float heading = i * 360 / (ring.count * 1.0) + ring.phase;
    bricks.push_back(BrickSpec()
                     .setElevation(ring.elevation)
                     .setHeading(heading)
                     .setColor(ring.color)
                     .setValue(ring.value)
                     .setLives(ring.lives)
                     .setSpeed(ring.speed)
                     .setStopHeading(heading + ring.stopHeading));
  }
}

std::vector<BrickSpec> RoundConfig::allBricks() const {
  std::vector<BrickSpec> allBricks(_bricks);
  for (const BrickRingSpec& ring : _brickRings) {
    createRingBricks(ring, allBricks);
  }
  return allBricks;
}

static void createRingWalls(const WallRingSpec& ring, std::vector<WallSpec>& walls) {
  for (int i = 0; i < ring.count; i++) {
    float heading = i * 360 / (ring.count * 1.0) + ring.phase;
    walls.push_back(WallSpec()
                    .setElevation(ring.elevation)
                    .setHeading(heading)
                    .setSize(ring.size)
                    .setIsExit(ring.isExit)
                    .setSpeed(ring.speed)
                    .setStopHeading(heading + ring.stopHeading)
                    .setVisible(ring.visible));
  }
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
    walls.push_back(WallSpec()
                    .setElevation(theta)
                    .setHeading(phi)
                    .setSize(ofVec3f(curve.width))
                    .setIsExit(curve.isExit)
                    .setSpeed(curve.speed)
                    .setStopHeading(phi + curve.stopHeading)
                    .setVisible(false));
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
  for (const WallRingSpec& ring : _wallRings) {
    createRingWalls(ring, walls);
  }
  return walls;
}
