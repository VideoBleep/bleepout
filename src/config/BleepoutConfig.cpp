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

ofPtr<RoundConfig>
BleepoutConfig::getRound(const std::string &name) {
  for (const auto& round : _roundConfigs) {
    if (round->name() == name)
      return round;
  }
  return ofPtr<RoundConfig>();
}

BleepoutConfig* BleepoutConfig::createConfig() {
  BleepoutConfig* config = new BleepoutConfig();
  config->_syphonServerName = "Composition";
  config->_syphonAppName = "Arena";
  
  config->roundStartedSound = "";
  config->roundEndedSound = "";
  config->brickDestroyedSound = "";
  config->collisionSound = "";
  config->modifierAppliedSound = "";
  config->modifierRemovedSound = "";
  config->ballDestroyedSound = "";
  config->playerLivesChangedSound = "";
  config->playerLostSound = "";
  config->countdownTimerTickSound = "";
  
  config->_roundConfigs.push_back(ofPtr<RoundConfig>(RoundConfig::createRoundConfig1()));
  config->_roundConfigs.push_back(ofPtr<RoundConfig>(RoundConfig::createRoundConfig2()));
  config->_roundConfigs.push_back(ofPtr<RoundConfig>(RoundConfig::createRoundConfig3()));
  config->_roundConfigs.push_back(ofPtr<RoundConfig>(RoundConfig::createRoundConfig4()));
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
: _paddleSize(16.0f, 8.0f, 40.0f),
_ballRadius(8.0f),
_modifierRadius(9.0f),
_brickFadeTime(0.4f),
_modifierFadeTime(0.2f),
_ballSpawnedFadeTime(0.2f),
_domeRadius(150.0f),
_domeMargin(20.0f),
_name(name),
_startDelay(0),
countdownTimerPeriod(10) { }

void RoundConfig::loadJsonFile(std::string path) {
  Json::Value obj;
  if (!readJsonFile(path, &obj))
    return;
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
  BrickSpec prototype = BrickSpec()
    .setElevation(ring.elevation)
    .setSize(ring.size)
    .setColor(ring.color)
    .setValue(ring.value)
    .setLives(ring.lives)
    .setSpeed(ring.speed);
  for (int i = 0; i < ring.count; i++) {
    float heading = i * 360 / (ring.count * 1.0) + ring.phase;
    BrickSpec brick = BrickSpec()
      .copyFrom(prototype)
      .setHeading(heading)
      .setStopHeading(ring.stopHeading < 0 ? -1 : (heading + ring.stopHeading));
    if (!ring.modifierName.empty()) {
      if (ring.modifierChance >= 1 || ofRandomuf() >= ring.modifierChance) {
        brick.setModifier(ring.modifierName);
      }
    }
    bricks.push_back(brick);
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
  WallSpec prototype = WallSpec()
    .setElevation(ring.elevation)
    .setSize(ring.size)
    .setIsExit(ring.isExit)
    .setSpeed(ring.speed)
    .setVisible(ring.visible);
  for (int i = 0; i < ring.count; i++) {
    float heading = i * 360 / (ring.count * 1.0) + ring.phase;
    walls.push_back(WallSpec()
                    .copyFrom(prototype)
                    .setHeading(heading)
                    .setStopHeading(ring.stopHeading < 0 ? -1 : (heading + ring.stopHeading)));
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
  WallSpec prototype = WallSpec()
    .setSize(ofVec3f(curve.width))
    .setIsExit(curve.isExit)
    .setSpeed(curve.speed)
    .setVisible(false);
  for (int i = 0; i < steps; i++) {
    walls.push_back(WallSpec()
                    .copyFrom(prototype)
                    .setElevation(theta)
                    .setHeading(phi)
                    .setStopHeading(curve.stopHeading < 0 ? -1 : (phi + curve.stopHeading)));
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

GameRules::GameRules()
: _backup(NULL)
, _timeLimit()
, _playersCanLoseLives()
, _ballsRespawn() { }

GameRules::GameRules(const GameRules& other)
: _backup(other._backup)
, _timeLimit(other._timeLimit)
, _playersCanLoseLives(other._playersCanLoseLives)
, _ballsRespawn(other._ballsRespawn) { }

float GameRules::timeLimit() const {
  return _timeLimit.get(_backup ? &_backup->_timeLimit : NULL, -1);
}

bool GameRules::playersCanLoseLives() const {
  return _playersCanLoseLives.get(_backup ? &_backup->_playersCanLoseLives : NULL, false);
}

bool GameRules::ballsRespawn() const {
  return _ballsRespawn.get(_backup ? &_backup->_ballsRespawn : NULL, false);
}
