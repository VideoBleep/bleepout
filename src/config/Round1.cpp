//
//  Round1.cpp
//  bleepout
//
//  Created by tekt on 12/21/14.
//
//

#include "BleepoutConfig.h"

RoundConfig* RoundConfig::createRoundConfig1() {
  RoundConfig* config = new RoundConfig("Round1");
  config->_brickSize.set(7.0f, 5.0f, 17.0f);
  config->_paddleSize.set(16.0f, 8.0f, 40.0f);
  config->_ballRadius = 8.0f;
  config->_modifierRadius = 9.0f;
  
  for (int i = 0; i < 5; i ++) {
    config->addBall(BallSpec(30, ofRandom(360)));
  }
  
  std::string widePaddleName("widePaddle");
  config->addModifierDef(widePaddleName, MODIFIER_PADDLE_WIDTH)
    .setAmount(1.5)
    .setDuration(5.0)
    .setColor(ofColor::blue);
  
  std::string narrowPaddleName("narrowPaddle");
  config->addModifierDef(narrowPaddleName, MODIFIER_PADDLE_WIDTH)
    .setAmount(0.5)
    .setDuration(5.0)
    .setColor(ofColor::red);
  
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
  
  // Create the floor exit wall
  float d = (config->domeMargin() + config->domeRadius()) * 5;
  config->addWall()
    .setElevation(-10)
    .setHeading(0)
    .setSize(ofVec3f(d, 10, d))
    .setIsExit(true);
  
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
