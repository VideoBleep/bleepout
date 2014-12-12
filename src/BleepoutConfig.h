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
  
  const std::vector<ofColor>& playerColors() const { return _playerColors; }
  
  const ofColor& getPlayerColor(int i) const {
    if (_playerColors.empty() || i >= _playerColors.size()) {
      return ofColor::black;
    }
    return _playerColors[i];
  }
  
  const BleepoutConfig& appConfig() const { return _appConfig; }
private:
  const BleepoutConfig& _appConfig;
  ofVec3f _brickSize;
  ofVec3f _paddleSize;
  float _ballRadius;
    
  float _domeRadius;
  float _domeMargin;
  
  std::vector<ofColor> _playerColors;
};

#endif /* defined(__bleepout__BleepoutConfig__) */
