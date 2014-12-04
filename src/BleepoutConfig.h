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
  static const BleepoutConfig& getInstance();
  
  void loadFile(const std::string& path);
  void saveFile(const std::string& path) const;
  
  int fps() const { return _fps; }
  ofLogLevel logLevel() const { return _logLevel; }
  bool vsync() const { return _vsync; }
private:
  int _fps;
  ofLogLevel _logLevel;
  bool _vsync;
};

class RoundConfig {
public:
  void loadFile(const std::string& path);
  void saveFile(const std::string& path) const;
  
  const ofVec2f& brickSize() const { return _brickSize; }
  float brickGap() const { return _brickGap; }
  const ofVec2f& paddleSize() const { return _paddleSize; }
  float ballRadius() const { return _ballRadius; }
  float ballDensity() const { return _ballDensity; }
  float ballBounce() const { return _ballBounce; }
  float ballFriction() const { return _ballFriction; }
  float paddleDensity() const { return _paddleDensity; }
  float paddleBounce() const { return _paddleBounce; }
  float paddleFriction() const { return _paddleFriction; }
private:
  ofVec2f _brickSize;
  float _brickGap;
  ofVec2f _paddleSize;
  float _ballDensity;
  float _ballBounce;
  float _ballFriction;
  float _ballRadius;
  float _paddleDensity;
  float _paddleBounce;
  float _paddleFriction;
};

#endif /* defined(__bleepout__BleepoutConfig__) */
