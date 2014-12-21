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

BleepoutConfig* BleepoutConfig::createConfig() {
  BleepoutConfig* config = new BleepoutConfig();
  config->_syphonServerName = "Composition";
  config->_syphonAppName = "Arena";
  config->_roundConfigs.push_back(ofPtr<RoundConfig>(RoundConfig::createRoundConfig1()));
  config->_roundConfigs.push_back(ofPtr<RoundConfig>(RoundConfig::createRoundConfig2()));
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
: _brickSize(7.0f, 5.0f, 17.0f),
_paddleSize(16.0f, 8.0f, 40.0f),
_ballRadius(8.0f),
_modifierRadius(9.0f),
_brickFadeTime(0.4f),
_domeRadius(150.0f),
_domeMargin(20.0f),
_name(name),
_startDelay(0) { }

void RoundConfig::loadJsonFile(std::string path) {
  Json::Value obj;
  if (!readJsonFile(path, &obj))
    return;
  readJsonVal(obj["brickSize"], &_brickSize);
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
  obj["brickSize"] = ::toJsonVal(_brickSize);
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
  for (int i = 0; i < ring.count; i++) {
    bricks.push_back(BrickSpec(ring.elevation, i * 360 / (ring.count * 1.0) + ring.phase, ring.color, ring.value, ring.lives, ring.speed));
  }
}

std::vector<BrickSpec> RoundConfig::allBricks() const {
  std::vector<BrickSpec> allBricks(_bricks);
  for (const BrickRingSpec& ring : _brickRings) {
    createRingBricks(ring, allBricks);
  }
  return allBricks;
}

static void createCurveWalls(const CurvedWallSpec& curve, float r, std::vector<WallSpec>& walls) {
  float theta = curve.elevation1;
  float phi = curve.heading1;
  float dtheta = curve.elevation2 - curve.elevation1;
  float dphi = curve.heading2 - curve.heading1;
  int steps = floor(max((r * dtheta * PI/180.0) / curve.width, (r * dphi * PI/180.0) / curve.width));
  dtheta /= steps * 1.0;
  dphi /= steps * 1.0;
  for (int i = 0; i < steps; i++) {
    walls.push_back(WallSpec(theta, phi, ofVec3f(curve.width), curve.isExit, curve.speed));
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
  return walls;
}

RoundConfig* RoundConfig::createRoundConfig1() {
  RoundConfig* config = new RoundConfig("Round1");
  
  for (int i = 0; i < 5; i ++) {
    config->addBall(BallSpec(30, ofRandom(360)));
  }
  
  std::string widePaddleName("widePaddle");
  config->addModifierDef(widePaddleName, MODIFIER_PADDLE_WIDTH)
    .setAmount(1.5)
    .setDuration(5.0);
  
  std::string narrowPaddleName("narrowPaddle");
  config->addModifierDef(narrowPaddleName, MODIFIER_PADDLE_WIDTH)
    .setAmount(0.5)
    .setDuration(5.0);
  
  int cols = 12;
  int rows = 10;
  
  for (int col = 0; col < cols; col++) {
    for (int row = 0; row < rows; row++) {
      float s = col / (cols * 1.0);
      auto& spec = config->addBrick()
        .setElevation(30 + 3 * row)
        .setHeading(s * 360 + row * 2 + ((col % 2) ? 5 : -5))
        .setColor(ofColor(s * 255,
                          row / (rows * 1.0) * 255,
                          (1 - s) * 255))
        .setLives((row % 3 == 1) ? 2 : 1)
        .setValue(1)
        .setSpeed(0);
      if (col % 3 == 0 && row % 3 == 0)
        spec.modifierName = widePaddleName;
      else if (col % 7 == 0 && row % 5 == 0)
        spec.modifierName = narrowPaddleName;
    }
  }
  
  for (int i = 0; i < 6; i++) {
    config->addCurvedWallSet()
      .setEnd1(30, i * 60 + 15)
      .setEnd2(i % 2 ? 68 : 62, i * 60 + 45)
      .setWidth(10);
  }
  
  // rotating top walls
  config->addWall()
    .setElevation(67)
    .setHeading(5)
    .setSize(ofVec3f(10, 10, 30))
    .setSpeed(0.02)
    .setStopHeading(80);
  config->addWall()
    .setElevation(67)
    .setHeading(125)
    .setSize(ofVec3f(10, 10, 30))
    .setSpeed(0.02)
    .setStopHeading(200);
  config->addWall()
    .setElevation(67)
    .setHeading(245)
    .setSize(ofVec3f(10, 10, 30))
    .setSpeed(0.02)
    .setStopHeading(320);
  
  // rotating top brick rings
  config->addBrickRing()
    .setElevation(72)
    .setColor(ofColor::black)
    .setCount(12)
    .setValue(2)
    .setLives(2)
    .setSpeed(0.02);
  config->addBrickRing()
    .setElevation(76)
    .setColor(ofColor::black)
    .setCount(10)
    .setSpeed(-0.02);
  config->addBrickRing()
    .setElevation(80)
    .setColor(ofColor::black)
    .setCount(8)
    .setValue(2)
    .setLives(2)
    .setSpeed(0.02);
  
  config->addStartMessage("Video Bleep\npresents", ofColor(255))
    .setSize(12)
    .setTiming(0, 3);
  config->addStartMessage("BLEEPOUT", ofColor(0, 120, 240))
    .setSize(50)
    .setTrails(4)
    .setTiming(3, 4.5);
  config->addStartMessage("STAGE 1 START", ofColor(0, 255, 0))
    .setSize(25)
    .setTiming(7.5, 2.5);
  config->_startDelay = 10;
  
  config->addRingSet()
    .setSpin(ValuePulserSpec<ofVec3f>(0, 0.3, 5.0f, ofVec3f(0)))
    .setSpread(ValuePulserSpec<ofVec3f>(0, 0.1f, 10.0f, ofVec3f(20)),
               ofVec3f(20))
    .setCount(30)
    .setRadiusScale(1.95)
    .setLineWidth(1.2)
    .setColor(ofColor(0, 0, 255, 63));
  config->addRingSet()
    .setSpin(ValuePulserSpec<ofVec3f>(0, 0.4, 5.0f, ofVec3f(0)))
    .setSpread(ValuePulserSpec<ofVec3f>(0, 0.5, 40.0f, ofVec3f(0)),
               ofVec3f(60))
    .setCount(60)
    .setRadiusScale(2.3)
    .setLineWidth(1.4)
    .setColor(ofColor(0, 255, 255, 63));
  config->addRingSet()
    .setSpin(ValuePulserSpec<ofVec3f>(0, 0.2, 5.0f, ofVec3f(0)))
    .setSpread(ValuePulserSpec<ofVec3f>(0.01, 0.16, 10.0f, ofVec3f(0)),
               ofVec3f(60))
    .setCount(50)
    .setRadiusScale(2)
    .setLineWidth(0.9)
    .setColor(ofColor(127, 172, 255, 63));
  
  //...
  return config;
}
RoundConfig* RoundConfig::createRoundConfig2() {
  RoundConfig* config = new RoundConfig("Round2");
  
  for (int i = 0; i < 5; i ++) {
    config->addBall(BallSpec(30, ofRandom(360)));
  }
  //...
  config->addStartMessage("STAGE 2 START", ofColor(0, 255, 0))
    .setSize(25)
    .setTiming(0, 3);
  
  int cols = 14;
  int rows = 10;
  
  for (int col = 0; col < cols; col++) {
    for (int row = 0; row < rows; row++) {
      float s = col / (cols * 1.0);
      config->addBrick()
        .setElevation(30 + 3 * row)
        .setHeading(s * 360 + row * 6)
        .setColor((row % 3 == 1) ? ofColor::green : ofColor::brown)
        .setLives((row % 3 == 1) ? 2 : 1)
        .setValue(1)
        .setSpeed(0);
    }
  }
  return config;
}
