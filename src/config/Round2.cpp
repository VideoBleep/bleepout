//
//  Round2.cpp
//  bleepout
//
//  Created by tekt on 12/21/14.
//
//

#include "BleepoutConfig.h"

RoundConfig* RoundConfig::createRoundConfig2() {
  RoundConfig* config = new RoundConfig("Round2");
  config->_paddleSize.set(16.0f, 8.0f, 40.0f);
  config->_ballRadius = 8.0f;
  config->_modifierRadius = 9.0f;
  
  for (int i = 0; i < 5; i ++) {
    config->addBall(BallSpec(30, ofRandom(360)));
  }
  //...
  config->addStartMessage("Video Bleep\npresents", ofColor(255))
    .setSize(12)
    .setTiming(0, 3);
  config->addStartMessage("BLEEPOUT", ofColor(0, 120, 240))
    .setSize(50)
    .setTrails(4)
    .setTiming(3, 4.5);
  config->addStartMessage("STAGE 2 START", ofColor(0, 255, 255))
    .setSize(25)
    .setTiming(7.5, 3);
  
  int cols = 18;
  int rows = 10;
  
  for (int col = 0; col < cols; col++) {
    for (int row = 0; row < rows; row++) {
      float s = col / (cols * 1.0);
      config->addBrick()
      .setElevation(35 + 5 * row)
      .setHeading(s * 360 + row * 6)
      .setColor((row % 3 == 1) ? ofColor(0, 127, 255)
                : ofColor::green)
      .setLives((row % 3 == 1) ? 2 : 1)
      .setValue(1)
      .setSpeed(0)
      .setSize(ofVec3f(5.0f, 2.0f, 10.0f));
    }
  }
  
  // Create the floor exit wall
  float d = (config->domeMargin() + config->domeRadius()) * 5;
  config->addWall()
    .setElevation(-10)
    .setHeading(0)
    .setSize(ofVec3f(d, 10, d))
    .setIsExit(true);
  
  // bottom moving walls
  config->addWallRing()
    .setElevation(25)
    .setCount(5)
    .setSpeed(0.02)
    .setSize(ofVec3f(10, 10, 30));
  config->addWallRing()
    .setElevation(30)
    .setCount(5)
    .setSpeed(0.05)
    .setSize(ofVec3f(10, 10, 30));
  
  config->addRingSet()
    .setSpin(ValuePulserSpec<ofVec3f>(0, 0.01, 10.0, ofVec3f(0)))
    .setSpread(ValuePulserSpec<ofVec3f>(0.2, 0.5, 10.0, ofVec3f(0.3)),
               ofVec3f(50, 90, 0))
    .setColor(ofColor(0, 0, 255, 63))
    .setCount(80)
    .setLineWidth(2.3)
    .setRadiusScale(2.3);
  
  return config;
}
