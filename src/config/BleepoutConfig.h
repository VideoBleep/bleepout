//
//  BleepoutConfig.h
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#ifndef __bleepout__BleepoutConfig__
#define __bleepout__BleepoutConfig__

#include <ofMain.h>
#include <string>
#include <vector>
#include <json.h> // it's included as part of ofxLibwebsockets
#include "ObjectSpecs.h"
#include "Common.h"

class JsonLoader;

class GameRules {
public:
  GameRules();
  explicit GameRules(const GameRules& other);
  
  GameRules& copyFrom(const GameRules& other);
  
  void readJson(const JsonLoader& loader, const Json::Value& obj);
  
  float timeLimit() const;
  void setTimeLimit(float value) { _timeLimit.set(value); }
  void unsetTimeLimit() { _timeLimit.unset(); }
  bool specifiesTimeLimit() const { return _timeLimit.hasValue(); }
  
  bool playersCanLoseLives() const;
  void setPlayersCanLoseLives(bool value) { _playersCanLoseLives.set(value); }
  void unsetPlayersCanLoseLives() { _playersCanLoseLives.unset(); }
  bool specifiesPlayersCanLoseLives() const { return _playersCanLoseLives.hasValue(); }
  
  bool ballsRespawn() const;
  void setBallsRespawn(bool value) { _ballsRespawn.set(value); }
  void unsetBallsRespawn() { _ballsRespawn.unset(); }
  bool specifiesBallsRespawn() const { return _ballsRespawn.hasValue(); }
  
  void setBackup(const GameRules* backup) { _backup = backup; }
private:
  const GameRules* _backup;
  Optional<float> _timeLimit;
  Optional<bool> _playersCanLoseLives;
  Optional<bool> _ballsRespawn;
};

class RoundConfig {
public:
  static RoundConfig* createRoundConfig1();
  static RoundConfig* createRoundConfig2();
  static RoundConfig* createRoundConfig3();
  static RoundConfig* createRoundConfig4();
  
  explicit RoundConfig(std::string name);
  
  void readJson(const JsonLoader& loader, const Json::Value& obj);
  
  void loadJsonFile(std::string path);
  void saveJsonFile(std::string path) const;
  
  const std::string& name() const { return _name; }
  
  float startDelay() const { return _startDelay; }
  const ofVec3f& paddleSize() const { return _paddleSize; }
  float ballRadius() const { return _ballRadius; }
  float brickFadeTime() const { return _brickFadeTime; }
  float modifierRadius() const { return _modifierRadius; }
  float modifierFadeTime() const { return _modifierFadeTime; }
  float ballSpawnedFadeTime() const { return _ballSpawnedFadeTime; }
  
  const std::vector<BallSpec>& balls() const { return _balls; }
  const std::vector<BrickSpec>& bricks() const { return _bricks; }
  const std::vector<BrickRingSpec>& brickRings() const { return _brickRings; }
  const std::vector<WallSpec>& walls() const { return _walls; }
  const std::vector<WallRingSpec>& wallRings() const { return _wallRings; }
  const std::vector<CurvedWallSpec>& curvedWallSets() const { return _curvedWallSets; }
  const std::vector<MessageSpec>& startMessages() const { return _startMessages; }
  const std::vector<RingSetSpec>& ringSets() const { return _ringSets; }
  
  std::vector<BallSpec>& balls() { return _balls; }
  std::vector<BrickSpec>& bricks() { return _bricks; }
  std::vector<BrickRingSpec>& brickRings() { return _brickRings; }
  std::vector<WallRingSpec>& wallRings() { return _wallRings; }
  std::vector<WallSpec>& walls() { return _walls; }
  std::vector<CurvedWallSpec>& curvedWallSets() { return _curvedWallSets; }
  std::vector<MessageSpec>& startMessages() { return _startMessages; }
  
  void addBall(BallSpec ball) {
    _balls.push_back(ball);
  }
  void addBrick(BrickSpec brick) {
    _bricks.push_back(brick);
  }
  BrickSpec& addBrick() {
    _bricks.push_back(BrickSpec());
    return _bricks.back();
  }
  void addBrickRing(BrickRingSpec ring) {
    _brickRings.push_back(ring);
  }
  BrickRingSpec& addBrickRing() {
    _brickRings.push_back(BrickRingSpec());
    return _brickRings.back();
  }
  void addWall(WallSpec wall) {
    _walls.push_back(wall);
  }
  WallSpec& addWall() {
    _walls.push_back(WallSpec());
    return _walls.back();
  }
  WallRingSpec& addWallRing() {
    _wallRings.push_back(WallRingSpec());
    return _wallRings.back();
  }
  void addCurvedWallSet(CurvedWallSpec curve) {
    _curvedWallSets.push_back(curve);
  }
  CurvedWallSpec& addCurvedWallSet() {
    _curvedWallSets.push_back(CurvedWallSpec());
    return _curvedWallSets.back();
  }
  void addModifierDef(std::string name, ModifierSpec spec) {
    _modifierDefs.insert(std::make_pair(name, spec));
  }
  
  void addStartMessage(MessageSpec spec) {
    _startMessages.push_back(spec);
  }
  MessageSpec& addStartMessage(std::string text, ofColor color) {
    _startMessages.push_back(MessageSpec(text, color));
    return _startMessages.back();
  }
  
  void addRingSet(RingSetSpec spec) {
    _ringSets.push_back(spec);
  }
  
  RingSetSpec& addRingSet() {
    _ringSets.push_back(RingSetSpec());
    return _ringSets.back();
  }
  
  BrickQuadsSpec& addBrickQuads() {
    _brickQuads.push_back(BrickQuadsSpec());
    return _brickQuads.back();
  }
  
  CurvedBrickColumnSpec& addCurvedBrickColumn() {
    _curvedBrickColumns.push_back(CurvedBrickColumnSpec());
    return _curvedBrickColumns.back();
  }
  
  ModifierSpec& addModifierDef(std::string name, ModifierType type) {
    _modifierDefs[name] = ModifierSpec(name, type);
    return _modifierDefs.at(name);
  }
  
  const ModifierSpec& modifierDef(std::string name) const {
    return _modifierDefs.at(name);
  }
  
  std::vector<BrickSpec> allBricks() const;
  
  std::vector<WallSpec> allWalls() const;
  
  const GameRules& rules() const { return _rules; }
  GameRules& rules() { return _rules; }
  
  float countdownTimerPeriod;
  
  Json::Value toJsonVal() const;
private:
  std::string _name;
  float _startDelay;
  ofVec3f _paddleSize;
  float _ballRadius;
  float _brickFadeTime;
  float _modifierRadius;
  float _modifierFadeTime;
  float _ballSpawnedFadeTime;
  GameRules _rules;
  
  std::vector<BallSpec> _balls;
  std::vector<BrickSpec> _bricks;
  std::vector<BrickRingSpec> _brickRings;
  std::vector<CurvedBrickColumnSpec> _curvedBrickColumns;
  std::vector<BrickQuadsSpec> _brickQuads;
  std::vector<WallSpec> _walls;
  std::vector<WallRingSpec> _wallRings;
  std::vector<CurvedWallSpec> _curvedWallSets;
  std::map<std::string, ModifierSpec> _modifierDefs;
  std::vector<MessageSpec> _startMessages;
  std::vector<RingSetSpec> _ringSets;
};

class BleepoutConfig {
public:
  static BleepoutConfig* createConfig();
  
  BleepoutConfig();
  
  void loadJsonFile(std::string path);
  void saveJsonFile(std::string path) const;
  
  int fps() const { return _fps; }
  ofLogLevel logLevel() const { return _logLevel; }
  bool vsync() const { return _vsync; }
  
  const std::string& syphonServerName() const { return _syphonServerName; }
  const std::string syphonAppName() const { return _syphonAppName; }
  
  const std::vector<ofPtr<RoundConfig> >& roundConfigs() const {
    return _roundConfigs;
  }
  
  ofPtr<RoundConfig> getRound(const std::string& name);
  
  std::string roundStartedSound;
  std::string roundEndedSound;
  std::string brickHitSound;
  std::string brickDestroyedSound;
  std::string collisionSound;
  std::string modifierAppliedSound;
  std::string modifierRemovedSound;
  std::string ballDestroyedSound;
  std::string playerLivesChangedSound;
  std::string playerLostSound;
  std::string countdownTimerTickSound;
  
  Json::Value toJsonVal() const;
private:
  int _fps;
  ofLogLevel _logLevel;
  bool _vsync;
  std::string _syphonServerName;
  std::string _syphonAppName;
  std::vector<ofPtr<RoundConfig> > _roundConfigs;
};

#endif /* defined(__bleepout__BleepoutConfig__) */
