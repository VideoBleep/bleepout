//
//  Round3.cpp
//  bleepout
//
//  Created by tekt on 12/21/14.
//
//

#include "BleepoutConfig.h"
#include "BleepoutParameters.h"

RoundConfig* RoundConfig::createRoundConfig3() {
  const auto& appParams = BleepoutParameters::get();
  RoundConfig* config = new RoundConfig("Round3");
  config->_ballRadius = 8.0f;
  config->_modifierSize.set(9.0f, 16.0f, 9.0f);
  
  for (int i = 0; i < 5; i ++) {
    config->addBall(BallSpec(30, ofRandom(360)));
  }
  
  {
    BrickQuadsSpec prototype = BrickQuadsSpec()
      .setColor(ofColor(0, 255, 0), ofColor(0, 127, 127))
      .setSize(ofVec3f(7, 5, 14));
    
    config->addBrickQuads()
      .copyFrom(prototype)
      .setElevation(45, 1.5)
      .setCount(12)
      .setHeadingSpacing(4);
    config->addBrickQuads()
      .copyFrom(prototype)
      .setElevation(55, 1.5)
      .setCount(12)
      .setHeadingSpacing(4);
    config->addBrickQuads()
      .copyFrom(prototype)
      .setElevation(65, 1.5)
      .setCount(3)
      .setHeadingSpacing(8);
    config->addBrickQuads()
      .copyFrom(prototype)
      .setElevation(75, 1.5)
      .setCount(3)
      .setHeadingSpacing(12);
  }
  
  config->addBrickRing()
    .setElevation(30)
    .setCount(24)
    .setColor(ofColor(0, 255, 255))
    .setSize(ofVec3f(7.0f, 5.0f, 17.0f));
  
  config->addWallRing()
    .setElevation(65)
    .setPhase(60)
    .setCount(3)
    .setSize(ofVec3f(8, 3, 28))
    .setVisible(true);
  config->addWallRing()
    .setElevation(75)
    .setPhase(60)
    .setCount(3)
    .setSize(ofVec3f(8, 3, 26))
    .setVisible(true);
  config->addWallRing()
    .setElevation(60)
    .setPhase(360 / 24.0)
    .setCount(12)
    .setSize(ofVec3f(8, 3, 25))
    .setVisible(true);
  config->addWallRing()
    .setElevation(55)
    .setPhase(360 / 24.0)
    .setCount(12)
    .setSize(ofVec3f(8, 3, 20))
    .setVisible(true);
  config->addWallRing()
    .setElevation(50)
    .setPhase(360 / 24.0)
    .setCount(12)
    .setSize(ofVec3f(8, 3, 15))
    .setVisible(true);
  config->addWallRing()
    .setElevation(45)
    .setPhase(360 / 24.0)
    .setCount(12)
    .setSize(ofVec3f(8, 3, 10))
    .setVisible(true);
  
  // moving lower walls
  config->addWallRing()
    .setElevation(35)
    .setCount(6)
    .setSize(ofVec3f(8, 3, 25))
    .setVisible(true)
    .setSpeed(0.007);
  
  // Create the floor exit wall
  float d = (appParams.domeMargin + appParams.domeRadius) * 5;
  config->addWall()
    .setElevation(-10)
    .setHeading(0)
    .setSize(ofVec3f(d, 10, d))
    .setIsExit(true)
    .setVisible(false);
  
  config->addStartMessage("STAGE 3 START", ofColor(0, 255, 0))
    .setSize(25)
    .setTiming(1, 3);
  config->_startDelay = 5;
  
  config->addRingSet()
    .setSpin(ValuePulserSpec<ofVec3f>(0, 0.3, 5.0f, ofVec3f(0)))
    .setSpread(ValuePulserSpec<ofVec3f>(0, 0.1f, 10.0f, ofVec3f(20)),
               ofVec3f(20))
    .setCount(30)
    .setRadiusScale(1.95)
    .setLineWidth(1.2)
    .setColor(ofColor(0, 255, 0, 63));
  config->addRingSet()
    .setSpin(ValuePulserSpec<ofVec3f>(0, 0.4, 5.0f, ofVec3f(0)))
    .setSpread(ValuePulserSpec<ofVec3f>(0, 0.5, 40.0f, ofVec3f(0)),
               ofVec3f(60))
    .setCount(60)
    .setRadiusScale(2.3)
    .setLineWidth(1.4)
    .setColor(ofColor(0, 255, 63, 63));
  config->addRingSet()
    .setSpin(ValuePulserSpec<ofVec3f>(0, 0.2, 10.0f, ofVec3f(0.2)))
    .setSpread(ValuePulserSpec<ofVec3f>(0.01, 0.16, 15.0f, ofVec3f(0.1)),
               ofVec3f(60))
    .setCount(50)
    .setRadiusScale(2)
    .setLineWidth(0.9)
    .setColor(ofColor(0, 127, 255, 63));
  
  return config;
}
