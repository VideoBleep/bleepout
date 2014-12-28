//
//  Round4.cpp
//  bleepout
//
//  Created by tekt on 12/26/14.
//
//

#include "BleepoutConfig.h"

RoundConfig* RoundConfig::createRoundConfig4() {
  RoundConfig* config = new RoundConfig("Round 4");
  config->_paddleSize.set(16, 8, 40);
  config->_ballRadius = 8;
  config->_modifierRadius = 9;
  
  for (int i = 0; i < 5; i++) {
    config->addBall(BallSpec(30, ofRandom(360)));
  }
  
  std::string laserModName("laser");
  config->addModifierDef(laserModName, MODIFIER_LASER_BALL)
    .setColor(ofColor::green)
    .setDuration(20);
  
  
  BrickQuadClusterRingsSpec rings;
  rings
    .setColor(ofColor(0, 255, 0), ofColor(0, 127, 127))
    .setSize(ofVec3f(7, 5, 14))
    .setModifier(laserModName, .7);
  
  rings.setElevation(45, 1.5)
    .setCount(12)
    .setHeadingSpacing(4)
    .addBricksTo(config);
  rings.setElevation(55, 1.5)
    .setCount(12)
    .setHeadingSpacing(4)
    .addBricksTo(config);
  rings.setElevation(65, 1.5)
    .setCount(3)
    .setHeadingSpacing(8)
    .addBricksTo(config);
  rings.setElevation(75, 1.5)
    .setCount(3)
    .setHeadingSpacing(12)
    .addBricksTo(config);
  
  config->addBrickRing()
    .setElevation(60)
    .setCount(24)
    .setColor(ofColor(0, 0, 192))
    .setSize(ofVec3f(7, 5, 8));
  config->addBrickRing()
    .setElevation(60)
    .setCount(24)
    .setColor(ofColor(0, 192, 255))
    .setSize(ofVec3f(7, 5, 8))
    .setValue(2)
    .setLives(2)
    .setPhase(8);
  
  config->addWallRing()
    .setElevation(56)
    .setCount(12)
    .setVisible(true)
    .setIsExit(false)
    .setSpeed(0.02)
    .setSize(ofVec3f(7, 5, 15));
  
  config->addBrickRing()
    .setElevation(52)
    .setCount(28)
    .setColor(ofColor(0, 0, 192))
    .setSize(ofVec3f(7, 5, 10));
  config->addBrickRing()
    .setElevation(52)
    .setCount(28)
    .setColor(ofColor(0, 192, 255))
    .setSize(ofVec3f(7, 5, 10))
    .setValue(2)
    .setLives(2)
    .setPhase(6);
  
  config->addWallRing()
    .setElevation(48)
    .setCount(24)
    .setVisible(true)
    .setIsExit(false)
    .setSpeed(0)
    .setSize(ofVec3f(7, 5, 12));
  
  config->addBrickRing()
    .setElevation(44)
    .setCount(12)
    .setLives(2)
    .setValue(2)
    .setColor(ofColor(0, 255, 127))
    .setSpeed(0.005)
    .setModifier(laserModName, .5);
  
  config->addBrickRing()
    .setElevation(42)
    .setCount(28)
    .setColor(ofColor(0, 0, 192))
    .setSize(ofVec3f(7, 5, 10));
  config->addBrickRing()
    .setElevation(42)
    .setCount(28)
    .setColor(ofColor(0, 192, 255))
    .setSize(ofVec3f(7, 5, 10))
    .setValue(2)
    .setLives(2)
    .setPhase(6);
  
  config->addWallRing()
    .setElevation(38)
    .setCount(6)
    .setVisible(true)
    .setIsExit(false)
    .setSpeed(0.05)
    .setSize(ofVec3f(7, 5, 8));
  
  config->addWallRing()
    .setElevation(36)
    .setCount(12)
    .setVisible(true)
    .setIsExit(false)
    .setSpeed(0)
    .setSize(ofVec3f(7, 5, 12));
  config->addWallRing()
    .setElevation(34)
    .setCount(12)
    .setVisible(true)
    .setIsExit(false)
    .setSpeed(0)
    .setSize(ofVec3f(7, 5, 10));
  config->addWallRing()
    .setElevation(32)
    .setCount(12)
    .setVisible(true)
    .setIsExit(false)
    .setSpeed(0)
    .setSize(ofVec3f(7, 5, 8));
  
  // Create the floor exit wall
  float d = (config->domeMargin() + config->domeRadius()) * 5;
  config->addWall()
    .setElevation(-10)
    .setHeading(0)
    .setSize(ofVec3f(d, 10, d))
    .setIsExit(true)
    .setVisible(false);
  
  config->addStartMessage("Video Bleep\npresents", ofColor(255))
    .setSize(12)
    .setTiming(0, 3);
  config->addStartMessage("BLEEPOUT", ofColor(0, 120, 240))
    .setSize(50)
    .setTrails(4)
    .setTiming(3, 4.5);
  config->addStartMessage("STAGE 4 START", ofColor(0, 255, 0))
    .setSize(25)
    .setTiming(7.5, 2.5);
  config->_startDelay = 10;
  
  config->addRingSet()
    .setSpin(ValuePulserSpec<ofVec3f>(0, 0.3, 5.0f, ofVec3f(0)))
    .setSpread(ValuePulserSpec<ofVec3f>(0, 0.1f, 10.0f, ofVec3f(20)),
               ofVec3f(20))
    .setCount(60)
    .setRadiusScale(1.95)
    .setLineWidth(2.2)
    .setColor(ofColor(0, 0, 255, 127));
  config->addRingSet()
    .setSpin(ValuePulserSpec<ofVec3f>(0, 0.4, 5.0f, ofVec3f(0)))
    .setSpread(ValuePulserSpec<ofVec3f>(0, 0.5, 40.0f, ofVec3f(0)),
               ofVec3f(60))
    .setCount(90)
    .setRadiusScale(2.3)
    .setLineWidth(2.1)
    .setColor(ofColor(0, 63, 192, 63));
  config->addRingSet()
    .setSpin(ValuePulserSpec<ofVec3f>(0, 0.2, 5.0f, ofVec3f(0)))
    .setSpread(ValuePulserSpec<ofVec3f>(0.01, 0.16, 10.0f, ofVec3f(0)),
               ofVec3f(60))
    .setCount(50)
    .setRadiusScale(2)
    .setLineWidth(2.9)
    .setColor(ofColor(0, 127, 255, 127));
  
  return config;
}
