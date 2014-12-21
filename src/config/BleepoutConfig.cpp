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
  ModifierSpec widePaddleSpec(MODIFIER_PADDLE_WIDTH);
  widePaddleSpec.amount = 1.5f;
  widePaddleSpec.duration = 5.0f;
  config->addModifierDef(widePaddleName, widePaddleSpec);
  
  std::string narrowPaddleName("narrowPaddle");
  ModifierSpec narrowPaddleSpec(MODIFIER_PADDLE_WIDTH);
  narrowPaddleSpec.amount = 0.5f;
  narrowPaddleSpec.duration = 5.0f;
  config->addModifierDef(narrowPaddleName, narrowPaddleSpec);
  
  int cols = 12;
  int rows = 10;
  
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++) {
      float s = i / (cols * 1.0);
      BrickSpec spec;
      spec.elevation = 30 + 3 * j;
      spec.heading = s * 360 + j * 2 + ((i % 2) ? 5 : -5);
      spec.color = ofColor(s * 255,
                           j / (rows * 1.0) * 255,
                           (1 - s) * 255);
      spec.lives = (j % 3 == 1) ? 2 : 1;
      spec.value = 1;
      spec.speed = 0;
      if (i % 3 == 0 && j % 3 == 0)
        spec.modifierName = widePaddleName;
      else if (i % 7 == 0 && j % 5 == 0)
        spec.modifierName = narrowPaddleName;
      config->addBrick(spec);
    }
  }
  
  for (int i = 0; i < 6; i++) {
    CurvedWallSpec spec;
    spec.elevation1 = 30;
    spec.heading1 = i * 60 + 15;
    spec.elevation2 = i % 2 ? 68 : 62;
    spec.heading2 = i * 60 + 45;
    spec.width = 10;
    spec.isExit = false;
    spec.speed = 0;
    config->addCurvedWallSet(spec);
  }
    
  config->addWall(WallSpec(67,   5, ofVec3f(10, 10, 30),
                          false, 0.02,  80));
  config->addWall(WallSpec(67, 125, ofVec3f(10, 10, 30),
                          false, 0.02, 200));
  config->addWall(WallSpec(67, 245, ofVec3f(10, 10, 30),
                          false, 0.02, 320));
  
  config->addBrickRing(BrickRingSpec(72, ofColor(0, 0, 0),
                                    12, 1, 2, 0, 0.02));
  config->addBrickRing(BrickRingSpec(76, ofColor(0, 0, 0),
                                    10, 1, 1, 0, -0.02));
  config->addBrickRing(BrickRingSpec(80, ofColor(0, 0, 0),
                                    8, 2, 2, 0, 0.02));
//  MessageSpec(std::string txt, ofColor c, float s, int trl, float del, float dur)
//  : text(txt), color(c), size(s), trails(trl)
//  , delay(del), duration(dur) { }
  config->addStartMessage(MessageSpec("Video Bleep\npresents",
                                      ofColor(255), 12, 0, 0, 3));
  config->addStartMessage(MessageSpec("BLEEPOUT",
                                      ofColor(0, 120, 240), 50, 4, 3, 4.5));
  config->addStartMessage(MessageSpec("STAGE 1 START",
                                      ofColor(0, 255, 0), 25, 0, 7.5, 2.5));
  config->_startDelay = 10;
  
  
//  void setup(SpinPulser spinPulser, SpinPulser spreadPulser, ofVec3f spreadOffset, int count, float radiusScale, float lineWidth, ofColor color)
  {
    RingSetSpec spec;
    spec.spin = SpinPulserSpec(0, 0.3, 5.0f, ofVec3f(0));
    spec.spread = SpinPulserSpec(0, 0.1f, 10.0f, ofVec3f(20));
    spec.spreadOffset.set(20);
    spec.count = 30;
    spec.radiusScale = 1.95;
    spec.lineWidth = 1.2;
    spec.color.set(0, 0, 255, 63);
    config->addRingSet(spec);
  }
  {
    RingSetSpec spec;
    spec.spin = SpinPulserSpec(0, 0.4, 5.0f, ofVec3f(0));
    spec.spread = SpinPulserSpec(0, 0.5, 40.0f, ofVec3f(0));
    spec.spreadOffset.set(60);
    spec.count = 60;
    spec.radiusScale = 2.3;
    spec.lineWidth = 1.4;
    spec.color.set(0, 255, 255, 63);
    config->addRingSet(spec);
  }
  {
    RingSetSpec spec;
    spec.spin = SpinPulserSpec(0, 0.2, 5.0f, ofVec3f(0));
    spec.spread = SpinPulserSpec(0.01, 0.16, 10.0f, ofVec3f(0));
    spec.spreadOffset.set(60);
    spec.count = 50;
    spec.radiusScale = 2;
    spec.lineWidth = 0.9;
    spec.color.set(127, 172, 255, 63);
    config->addRingSet(spec);
  }

  //...
  return config;
}
RoundConfig* RoundConfig::createRoundConfig2() {
  RoundConfig* config = new RoundConfig("Round2");
  //...
  config->addStartMessage(MessageSpec("STAGE 2 START", ofColor(0, 255, 0), 25, 0, 2, 2.5));
  return config;
}
