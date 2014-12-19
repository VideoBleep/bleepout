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
#include "JsonUtil.h"

class BleepoutConfig {
public:
  static BleepoutConfig createTestConfig();
  
  BleepoutConfig();
  BleepoutConfig(const BleepoutConfig& other);
  
  BleepoutConfig& operator=(const BleepoutConfig& other);
  
  void loadJsonFile(std::string path);
  void saveJsonFile(std::string path) const;
  
  int fps() const { return _fps; }
  ofLogLevel logLevel() const { return _logLevel; }
  bool vsync() const { return _vsync; }
  
  const std::string& syphonServerName() const { return _syphonServerName; }
  const std::string syphonAppName() const { return _syphonAppName; }
  
  Json::Value toJsonVal() const;
private:
  int _fps;
  ofLogLevel _logLevel;
  bool _vsync;
  std::string _syphonServerName;
  std::string _syphonAppName;
};

class RoundConfig {
public:
  
  static RoundConfig createTestConfig();
  
  RoundConfig();
  
  void loadJsonFile(std::string path);
  void saveJsonFile(std::string path) const;
  
  const ofVec3f& brickSize() const { return _brickSize; }
  const ofVec3f& paddleSize() const { return _paddleSize; }
  float ballRadius() const { return _ballRadius; }
  float brickFadeTime() const { return _brickFadeTime; }
  float modifierRadius() const { return _modifierRadius; }
  
  float domeRadius() const { return _domeRadius; }
  float domeMargin() const { return _domeMargin; }
  
  const std::vector<BallSpec>& balls() const { return _balls; }
  const std::vector<BrickSpec>& bricks() const { return _bricks; }
  const std::vector<BrickRingSpec>& brickRings() const { return _brickRings; }
  const std::vector<WallSpec>& walls() const { return _walls; }
  const std::vector<CurvedWallSpec>& curvedWallSets() const { return _curvedWallSets; }
  
  std::vector<BallSpec>& balls() { return _balls; }
  std::vector<BrickSpec>& bricks() { return _bricks; }
  std::vector<BrickRingSpec>& brickRings() { return _brickRings; }
  std::vector<WallSpec>& walls() { return _walls; }
  std::vector<CurvedWallSpec>& curvedWallSets() { return _curvedWallSets; }
  
  void addBall(BallSpec ball) {
    _balls.push_back(ball);
  }
  void addBrick(BrickSpec brick) {
    _bricks.push_back(brick);
  }
  void addBrickRing(BrickRingSpec ring) {
    _brickRings.push_back(ring);
  }
  void addWall(WallSpec wall) {
    _walls.push_back(wall);
  }
  void addCurvedWallSet(CurvedWallSpec curve) {
    _curvedWallSets.push_back(curve);
  }
  void addModifierDef(std::string name, ModifierSpec spec) {
    _modifierDefs.insert(std::make_pair(name, spec));
  }
  
  const ModifierSpec& modifierDef(std::string name) const {
    return _modifierDefs.at(name);
  }
  
  std::vector<BrickSpec> allBricks() const;
  
  std::vector<WallSpec> allWalls() const;
  
  Json::Value toJsonVal() const;
private:
  ofVec3f _brickSize;
  ofVec3f _paddleSize;
  float _ballRadius;
  float _brickFadeTime;
  float _modifierRadius;
    
  float _domeRadius;
  float _domeMargin;
  
  std::vector<BallSpec> _balls;
  std::vector<BrickSpec> _bricks;
  std::vector<BrickRingSpec> _brickRings;
  std::vector<WallSpec> _walls;
  std::vector<CurvedWallSpec> _curvedWallSets;
  std::map<std::string, ModifierSpec> _modifierDefs;
};

#endif /* defined(__bleepout__BleepoutConfig__) */
