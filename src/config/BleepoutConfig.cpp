//
//  BleepoutConfig.cpp
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#include "BleepoutConfig.h"
#include "BleepoutApp.h"
#include "BleepoutParameters.h"
#include "JsonUtil.h"

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

std::vector<BrickSpec> RoundConfig::allBricks() const {
  std::vector<BrickSpec> allBricks(_bricks);
  buildAllSpecs(*this, _brickRings, &allBricks);
  buildAllSpecs(*this, _brickQuads, &allBricks);
  buildAllSpecs(*this, _curvedBrickColumns, &allBricks);
  return allBricks;
}

std::vector<WallSpec> RoundConfig::allWalls() const {
  const auto& appParams = BleepoutParameters::get();
  std::vector<WallSpec> walls(_walls);
  buildAllSpecs(*this, _curvedWallSets, &walls);
  buildAllSpecs(*this, _wallRings, &walls);
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

GameRules& GameRules::copyFrom(const GameRules &other) {
  _timeLimit = other._timeLimit;
  _playersCanLoseLives = other._playersCanLoseLives;
  _ballsRespawn = other._ballsRespawn;
  return *this;
}

float GameRules::timeLimit() const {
  return _timeLimit.get(_backup ? &_backup->_timeLimit : NULL, -1);
}

bool GameRules::playersCanLoseLives() const {
  return _playersCanLoseLives.get(_backup ? &_backup->_playersCanLoseLives : NULL, false);
}

bool GameRules::ballsRespawn() const {
  return _ballsRespawn.get(_backup ? &_backup->_ballsRespawn : NULL, false);
}

void GameRules::readJson(const JsonLoader &loader,
                         const Json::Value &obj) {
  if (!loader.assertType(obj, Json::objectValue))
    return;
  loader.readVal(obj["timeLimit"], &_timeLimit);
  loader.readVal(obj["playersCanLoseLives"], &_playersCanLoseLives);
  loader.readVal(obj["ballsRespawn"], &_ballsRespawn);
}

void RoundConfig::readJson(const JsonLoader &loader,
                           const Json::Value &obj) {
  if (!loader.assertType(obj, Json::objectValue))
    return;
  loader.readVal(obj["name"], &_name, _name);
  loader.readVal(obj["startDelay"], &_startDelay);
  loader.readVal(obj["paddleSize"], &_paddleSize);
  loader.readVal(obj["ballRadius"], &_ballRadius);
  loader.readVal(obj["brickFadeTime"], &_brickFadeTime);
  loader.readVal(obj["modifierRadius"], &_modifierRadius);
  loader.readVal(obj["modifierFadeTime"], &_modifierFadeTime);
  loader.readVal(obj["ballSpawnedFadeTime"], &_ballSpawnedFadeTime);
//  loader.readVal(obj["rules"], &_rules);
  _rules.readJson(loader, obj["rules"]);
  loader.readArr(obj["balls"], &_balls);
  loader.readArr(obj["bricks"], &_bricks);
  loader.readArr(obj["brickRings"], &_brickRings);
  loader.readArr(obj["curvedBrickColumns"], &_curvedBrickColumns);
  loader.readArr(obj["brickQuads"], &_brickQuads);
  loader.readArr(obj["walls"], &_walls);
  loader.readArr(obj["wallRings"], &_wallRings);
  loader.readArr(obj["curvedWallSets"], &_curvedWallSets);
  std::vector<ModifierSpec> mods;
  loader.readArr(obj["modifierDefs"], &mods);
  _modifierDefs.clear();
  for (auto& mod : mods) {
    if (!mod.name.empty())
      _modifierDefs[mod.name] = mod;
  }
  loader.readArr(obj["startMessages"], &_startMessages);
  loader.readArr(obj["ringSets"], &_ringSets);
}

static RoundConfig* loadRoundFromObj(const JsonLoader& loader,
                                     const Json::Value& obj) {
  Json::Value name = obj["name"];
  if (!name.isString())
    return NULL;
  RoundConfig* config = new RoundConfig(name.asString());
  config->readJson(loader, obj);
  return config;
}

RoundConfig* RoundConfig::loadFromFile(std::string path) {
  JsonLoader loader;
  Json::Value obj;
  if (!loader.readFile(path, &obj))
    return NULL;
  return loadRoundFromObj(loader, obj);
}

static RoundConfig* loadRoundFromSpecifier(const JsonLoader& loader,
                                           const Json::Value& specifier) {
  if (specifier.isString()) {
    return RoundConfig::loadFromFile(specifier.asString());
  }
  if (!loader.assertType(specifier, Json::objectValue))
    return NULL;
  return loadRoundFromObj(loader, specifier);
}

static void loadRoundConfigs(const JsonLoader& loader,
                             const Json::Value& arr,
                             std::vector<ofPtr<RoundConfig> >* rounds) {
  if (!arr.isArray())
    return;
  rounds->clear();
  for (int i = 0; i < arr.size(); i++) {
    RoundConfig* round = loadRoundFromSpecifier(loader, arr[i]);
    if (round)
      rounds->push_back(ofPtr<RoundConfig>(round));
  }
}

void BleepoutConfig::readJson(const JsonLoader &loader, const Json::Value &obj) {
  loader.readVal(obj["logLevel"], &_logLevel);
  loader.readVal(obj["fps"], &_fps);
  loader.readVal(obj["vsync"], &_vsync);
  loader.readVal(obj["syphonServerName"], &_syphonServerName);
  loader.readVal(obj["syphonAppName"], &_syphonAppName);
  loader.readVal(obj["roundStartedSound"], &roundStartedSound);
  loader.readVal(obj["roundEndedSound"], &roundEndedSound);
  loader.readVal(obj["brickHitSound"], &brickHitSound);
  loader.readVal(obj["brickDestroyedSound"], &brickDestroyedSound);
  loader.readVal(obj["collisionSound"], &collisionSound);
  loader.readVal(obj["modifierAppliedSound"], &modifierAppliedSound);
  loader.readVal(obj["modifierRemovedSound"], &modifierRemovedSound);
  loader.readVal(obj["ballDestroyedSound"], &ballDestroyedSound);
  loader.readVal(obj["playerLivesChangedSound"], &playerLivesChangedSound);
  loader.readVal(obj["playerLostSound"], &playerLostSound);
  loader.readVal(obj["countdownTimerTickSound"], &countdownTimerTickSound);
  
  loadRoundConfigs(loader, obj["rounds"], &_roundConfigs);
}

BleepoutConfig* BleepoutConfig::loadFromFile(std::string path) {
  JsonLoader loader;
  Json::Value obj;
  if (!loader.readFile(path, &obj))
    return NULL;
  BleepoutConfig* config = new BleepoutConfig();
  config->readJson(loader, obj);
  return config;
}
