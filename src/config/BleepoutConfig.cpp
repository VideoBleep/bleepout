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
: _fps(60)
, _logLevel(OF_LOG_NOTICE)
, _vsync(true)
, paddleSize(16, 8, 40) { }

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
  
  config->roundStartedSound = "sounds/StageStart.wav";
  config->roundEndedSound = "sounds/StageEnd.wav";
  config->brickHitSound = "sounds/BrickHit.wav";
  config->brickDestroyedSound = "sounds/BrickDest.wav";
  config->paddleHitSound = "sounds/PaddleHit.wav";
  config->wallHitSound = "sounds/WallHit.wav";
  config->modifierAppliedSound = "sounds/PowerupOn.wav";
  config->modifierRemovedSound = "sounds/PowerupOff.wav";
  config->ballDestroyedSound = "sounds/BallDie.wav";
  config->playerLivesChangedSound = "";
  config->playerLostSound = "";
  config->countdownTimerTickSound = "";
  
  config->_roundConfigs.push_back(ofPtr<RoundConfig>(RoundConfig::createRoundConfig1()));
  config->_roundConfigs.push_back(ofPtr<RoundConfig>(RoundConfig::createRoundConfig2()));
  config->_roundConfigs.push_back(ofPtr<RoundConfig>(RoundConfig::createRoundConfig3()));
  config->_roundConfigs.push_back(ofPtr<RoundConfig>(RoundConfig::createRoundConfig4()));
  return config;
}

void BleepoutConfig::saveJsonFile(std::string path) const {
  Json::Value obj = buildJson();
  cleanJsonVal(obj);
  writeJsonFile(path, obj);
}

RoundConfig::RoundConfig(std::string name)
: _ballRadius(8.0f),
_modifierSize(9.0f, 20.0f, 9.0f),
_brickFadeTime(0.4f),
_modifierFadeTime(0.2f),
_ballSpawnedFadeTime(0.2f),
_name(name),
_startDelay(0),
ballSpeed(0.6),
countdownTimerPeriod(10) { }

template<>
Json::Value toJsonVal(const RoundConfig& value) {
  return value.buildJson();
}

template<>
Json::Value toJsonVal(const ofPtr<RoundConfig>& value) {
  if (value)
    return toJsonVal(*value);
  return Json::Value::null;
}

void RoundConfig::saveJsonFile(std::string path) const {
  Json::Value obj = buildJson();
  cleanJsonVal(obj);
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
, _playersCanLoseLives() { }

GameRules::GameRules(const GameRules& other)
: _backup(other._backup)
, _timeLimit(other._timeLimit)
, _playersCanLoseLives(other._playersCanLoseLives) { }

GameRules& GameRules::copyFrom(const GameRules &other) {
  _timeLimit = other._timeLimit;
  _playersCanLoseLives = other._playersCanLoseLives;
  return *this;
}

float GameRules::timeLimit() const {
  return _timeLimit.get(_backup ? &_backup->_timeLimit : NULL, -1);
}

bool GameRules::playersCanLoseLives() const {
  return _playersCanLoseLives.get(_backup ? &_backup->_playersCanLoseLives : NULL, false);
}

void GameRules::readJson(const JsonLoader &loader,
                         const Json::Value &obj) {
  if (!loader.assertType(obj, Json::objectValue))
    return;
  loader.readVal(obj["timeLimit"], &_timeLimit);
  loader.readVal(obj["playersCanLoseLives"], &_playersCanLoseLives);
}

Json::Value GameRules::buildJson() const {
  Json::Value obj(Json::objectValue);
  obj["timeLimit"] = toJsonVal(_timeLimit);
  obj["playersCanLoseLives"] = toJsonVal(_playersCanLoseLives);
  return obj;
}

template<>
Json::Value toJsonVal(const GameRules& value) {
  return value.buildJson();
}

void RoundConfig::readSpecJson(const JsonLoader &loader,
                               const Json::Value &obj) {
  SpecType type;
  loader.readVal(obj["_type"], &type);
  switch (type) {
    case SPEC_BRICK:
      loader.readVal(obj, &addBrick());
      break;
    case SPEC_BRICK_RING:
      loader.readVal(obj, &addBrickRing());
      break;
    case SPEC_BRICK_QUADS:
      loader.readVal(obj, &addBrickQuads());
      break;
    case SPEC_WALL:
      loader.readVal(obj, &addWall());
      break;
    case SPEC_WALL_RING:
      loader.readVal(obj, &addWallRing());
      break;
    case SPEC_CURVED_BRICK_COLUMN:
      loader.readVal(obj, &addCurvedBrickColumn());
      break;
    case SPEC_CURVED_WALL:
      loader.readVal(obj, &addCurvedWallSet());
      break;
    case SPEC_BALL: {
      BallSpec spec;
      loader.readVal(obj, &spec);
      addBall(spec);
      break;
    }
    case SPEC_MODIFIER: {
      ModifierSpec spec;
      loader.readVal(obj, &spec);
      addModifierDef(spec.name, spec);
      break;
    }
    case SPEC_MESSAGE: {
      MessageSpec spec;
      loader.readVal(obj, &spec);
      addStartMessage(spec);
      break;
    }
    case SPEC_RING_SET:
      loader.readVal(obj, &addRingSet());
      break;
    case SPEC_UNKNOWN:
    default:
      //...
      break;
  }
}

void RoundConfig::readJson(const JsonLoader &loader,
                           const Json::Value &obj) {
  if (!loader.assertType(obj, Json::objectValue))
    return;
  loader.readVal(obj["name"], &_name, _name);
  loader.readVal(obj["startDelay"], &_startDelay);
  loader.readVal(obj["ballRadius"], &_ballRadius);
  loader.readVal(obj["ballSpeed"], &ballSpeed);
  loader.readVal(obj["brickFadeTime"], &_brickFadeTime);
  loader.readVal(obj["modifierSize"], &_modifierSize);
  loader.readVal(obj["modifierFadeTime"], &_modifierFadeTime);
  loader.readVal(obj["ballSpawnedFadeTime"], &_ballSpawnedFadeTime);
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
  const Json::Value& specsArr = obj["objects"];
  if (!specsArr.isNull()){
    if (loader.assertType(specsArr, Json::arrayValue)) {
      int size = specsArr.size();
      for (int i = 0; i < size; i++) {
        readSpecJson(loader, specsArr[i]);
      }
    }
  }
}

Json::Value RoundConfig::buildJson() const {
  Json::Value obj(Json::objectValue);
  obj["name"] = _name;
  obj["startDelay"] = _startDelay;
  obj["ballRadius"] = _ballRadius;
  obj["ballSpeed"] = ballSpeed;
  obj["brickFadeTime"] = _brickFadeTime;
  obj["modifierSize"] = toJsonVal(_modifierSize);
  obj["modifierFadeTime"] = _modifierFadeTime;
  obj["ballSpawnedFadeTime"] = _ballSpawnedFadeTime;
  obj["rules"] = _rules.buildJson();
  Json::Value specsArr(Json::arrayValue);
  addToJsonArr(&specsArr, _balls);
  addToJsonArr(&specsArr, _bricks);
  addToJsonArr(&specsArr, _brickRings);
  addToJsonArr(&specsArr, _curvedBrickColumns);
  addToJsonArr(&specsArr, _brickQuads);
  addToJsonArr(&specsArr, _walls);
  addToJsonArr(&specsArr, _wallRings);
  addToJsonArr(&specsArr, _curvedWallSets);
  addValuesToJsonArr(&specsArr, _modifierDefs);
  addToJsonArr(&specsArr, _startMessages);
  addToJsonArr(&specsArr, _ringSets);
  obj["objects"] = specsArr;
  return obj;
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
  if (!loader.readFile(path, &obj)) {
    ofLogWarning() << "unable to load round from file: " << path;
    return NULL;
  }
  ofLogWarning() << "loading round from file: " << path;
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
  loader.readVal(obj["paddleSize"], &paddleSize);
  loader.readVal(obj["syphonServerName"], &_syphonServerName);
  loader.readVal(obj["syphonAppName"], &_syphonAppName);
  loader.readVal(obj["roundStartedSound"], &roundStartedSound);
  loader.readVal(obj["roundEndedSound"], &roundEndedSound);
  loader.readVal(obj["brickHitSound"], &brickHitSound);
  loader.readVal(obj["brickDestroyedSound"], &brickDestroyedSound);
  loader.readVal(obj["paddleHitSound"], &paddleHitSound);
  loader.readVal(obj["wallHitSound"], &wallHitSound);
  loader.readVal(obj["modifierAppliedSound"], &modifierAppliedSound);
  loader.readVal(obj["modifierRemovedSound"], &modifierRemovedSound);
  loader.readVal(obj["ballDestroyedSound"], &ballDestroyedSound);
  loader.readVal(obj["playerLivesChangedSound"], &playerLivesChangedSound);
  loader.readVal(obj["playerLostSound"], &playerLostSound);
  loader.readVal(obj["countdownTimerTickSound"], &countdownTimerTickSound);
  
  loadRoundConfigs(loader, obj["rounds"], &_roundConfigs);
}

BleepoutConfig* BleepoutConfig::loadFromFile(std::string path) {
  BleepoutConfig* config = new BleepoutConfig();
  if (!config->loadJsonFile(path)) {
    delete config;
    return NULL;
  }
  return config;
}

bool BleepoutConfig::loadJsonFile(std::string path) {
  JsonLoader loader;
  Json::Value obj;
  if (!loader.readFile(path, &obj))
    return false;
  readJson(loader, obj);
  return true;
}

Json::Value BleepoutConfig::buildJson() const {
  Json::Value obj(Json::objectValue);
  obj["logLevel"] = toJsonVal(_logLevel);
  obj["fps"] = _fps;
  obj["vsync"] = _vsync;
  obj["paddleSize"] = toJsonVal(paddleSize);
  obj["syphonServerName"] = _syphonServerName;
  obj["syphonAppName"] = _syphonAppName;
  obj["roundStartedSound"] = roundStartedSound;
  obj["roundEndedSound"] = roundEndedSound;
  obj["brickHitSound"] = brickHitSound;
  obj["brickDestroyedSound"] = brickDestroyedSound;
  obj["paddleHitSound"] = paddleHitSound;
  obj["wallHitSound"] = wallHitSound;
  obj["modifierAppliedSound"] = modifierAppliedSound;
  obj["modifierRemovedSound"] = modifierRemovedSound;
  obj["ballDestroyedSound"] = ballDestroyedSound;
  obj["playerLivesChangedSound"] = playerLivesChangedSound;
  obj["playerLostSound"] = playerLostSound;
  obj["countdownTimerTickSound"] = countdownTimerTickSound;
  obj["rounds"] = toJsonArr(_roundConfigs);
  return obj;
}
