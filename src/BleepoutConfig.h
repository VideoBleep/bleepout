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

class BleepoutConfig {
public:
  BleepoutConfig();
  
  void loadJsonFile(std::string path);
  void saveJsonFile(std::string path) const;
  
  int fps() const { return _fps; }
  ofLogLevel logLevel() const { return _logLevel; }
  bool vsync() const { return _vsync; }
private:
  int _fps;
  ofLogLevel _logLevel;
  bool _vsync;
};

struct BrickSpec {
  float elevation;
  float heading;
  ofColor color;
  
  BrickSpec() { }
  BrickSpec(float e, float h, ofColor c)
  : elevation(e), heading(h), color(c) { }
};

struct BrickRingSpec {
  float elevation;
  ofColor color;
  int count;
  float phase;
  
  BrickRingSpec() { }
  BrickRingSpec(float e, ofColor c, int n, float p = 0)
  : elevation(e), count(n), phase(p), color(c) { }
};

struct WallSpec {
  float elevation;
  float heading;
  ofVec3f size;
  
  WallSpec() { }
  WallSpec(float e, float h, ofVec3f s)
  : elevation(e), heading(h), size(s) { }
};

struct CurvedWallSpec {
  float elevation1;
  float heading1;
  float elevation2;
  float heading2;
  float width;
  CurvedWallSpec() { }
  CurvedWallSpec(float e1, float h1, float e2, float h2, float w)
  : elevation1(e1), heading1(h1), elevation2(e2), heading2(h2), width(w) { }
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
  
  RoundConfig(const BleepoutConfig& appConfig);
  
  void loadJsonFile(std::string path);
  void saveJsonFile(std::string path) const;
  
  const ofVec3f& brickSize() const { return _brickSize; }
  const ofVec3f& paddleSize() const { return _paddleSize; }
  float ballRadius() const { return _ballRadius; }
  
  float domeRadius() const { return _domeRadius; }
  float domeMargin() const { return _domeMargin; }
  
  const BleepoutConfig& appConfig() const { return _appConfig; }
private:
  const BleepoutConfig& _appConfig;
  ofVec3f _brickSize;
  ofVec3f _paddleSize;
  float _ballRadius;
    
  float _domeRadius;
  float _domeMargin;
};

#endif /* defined(__bleepout__BleepoutConfig__) */
