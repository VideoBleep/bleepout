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
#include "JsonUtil.h"

class BleepoutConfig {
public:
  static BleepoutConfig createTestConfig();
  
  BleepoutConfig();
  
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

struct BrickSpec {
  float elevation;
  float heading;
  int value;
  int lives;
  ofColor color;
  float speed;
  float stopHeading;
  
  BrickSpec() : speed(0), stopHeading(-1) { }
  BrickSpec(float e, float h, ofColor c, int v, int l, float s = 0, float stop = -1)
  : elevation(e), heading(h), color(c), value(v), lives(l), speed(s), stopHeading(stop) { }
};

struct BrickRingSpec {
  float elevation;
  ofColor color;
  int value;
  int lives;
  int count;
  float phase;
  float speed;
  float stopHeading;
  
  BrickRingSpec() : speed(0), stopHeading(-1) { }
  BrickRingSpec(float e, ofColor c, int n, int v, int l, float p = 0, float s = 0, float stop = -1)
  : elevation(e), count(n), phase(p), color(c), value(v), lives(l), speed(s), stopHeading(-1) { }
};

struct WallSpec {
  float elevation;
  float heading;
  ofVec3f size;
  bool isExit;
  float speed;
  float stopHeading;
  
  WallSpec() : speed(0), stopHeading(-1) { }
  WallSpec(float e, float h, ofVec3f s, bool exit = false, float sp = 0, float stop = -1)
  : elevation(e), heading(h), size(s), isExit(exit), speed(sp), stopHeading(stop) { }
};

struct CurvedWallSpec {
  float elevation1;
  float heading1;
  float elevation2;
  float heading2;
  float width;
  bool isExit;
  float speed;
  float stopHeading;
  
    CurvedWallSpec() : speed(0), stopHeading(-1) { }
  CurvedWallSpec(float e1, float h1, float e2, float h2, float w, bool exit = false, float sp = 0, float stop = -1)
  : elevation1(e1), heading1(h1), elevation2(e2), heading2(h2), width(w), isExit(exit), speed(sp), stopHeading(stop) { }
};

struct BallSpec {
  float elevation;
  float heading;
  
  BallSpec() { }
  BallSpec(float e, float h)
  : elevation(e), heading(h) { }
};

class RoundConfig {
public:
  
  static RoundConfig createTestConfig(const BleepoutConfig& appConfig);
  
  RoundConfig(const BleepoutConfig& appConfig);
  
  void loadJsonFile(std::string path);
  void saveJsonFile(std::string path) const;
  
  const ofVec3f& brickSize() const { return _brickSize; }
  const ofVec3f& paddleSize() const { return _paddleSize; }
  float ballRadius() const { return _ballRadius; }
  float brickFadeTime() const { return _brickFadeTime; }
  
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
  
  std::vector<BrickSpec> allBricks() const;
  
  std::vector<WallSpec> allWalls() const;
  
  const BleepoutConfig& appConfig() const { return _appConfig; }
  
  Json::Value toJsonVal() const;
private:
  const BleepoutConfig& _appConfig;
  ofVec3f _brickSize;
  ofVec3f _paddleSize;
  float _ballRadius;
  float _brickFadeTime;
    
  float _domeRadius;
  float _domeMargin;
  
  std::vector<BallSpec> _balls;
  std::vector<BrickSpec> _bricks;
  std::vector<BrickRingSpec> _brickRings;
  std::vector<WallSpec> _walls;
  std::vector<CurvedWallSpec> _curvedWallSets;
};

#endif /* defined(__bleepout__BleepoutConfig__) */
