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

struct PhysicsOptions {
  float density;
  float bounce;
  float friction;

  PhysicsOptions() {}
  PhysicsOptions(float d, float b, float f)
  : density(d), bounce(b), friction(f){}
};

class RoundConfig {
public:
  RoundConfig(const BleepoutConfig& appConfig);
  
  void loadJsonFile(std::string path);
  void saveJsonFile(std::string path) const;
  
  const ofVec3f& brickSize() const { return _brickSize; }
  float brickGap() const { return _brickGap; }
  const ofVec3f& paddleSize() const { return _paddleSize; }
  float ballRadius() const { return _ballRadius; }
  PhysicsOptions ballPhysics() const { return _ballPhysics; }
  PhysicsOptions paddlePhysics() const { return _paddlePhysics; }
  const ofVec3f& ballInitialVelocity() const { return _ballInitialVelocity; }
    
  float domeRadius() const { return _domeRadius; }
  float domeMargin() const { return _domeMargin; }
  
  const BleepoutConfig& appConfig() const { return _appConfig; }
private:
  const BleepoutConfig& _appConfig;
  ofVec3f _brickSize;
  float _brickGap;
  ofVec3f _paddleSize;
  PhysicsOptions _ballPhysics;
  PhysicsOptions _paddlePhysics;
  float _ballRadius;
  ofVec3f _ballInitialVelocity;
    
  float _domeRadius;
  float _domeMargin;
};

#endif /* defined(__bleepout__BleepoutConfig__) */
