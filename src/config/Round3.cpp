//
//  Round3.cpp
//  bleepout
//
//  Created by tekt on 12/21/14.
//
//

#include "BleepoutConfig.h"

static void createBrickGroups(RoundConfig* config,
                              float elevation,
                              int count,
                              float eSpace,
                              float hSpace) {
  ofColor color1(0, 255, 0);
  ofColor color2(0, 127, 127);
  config->addBrickRing()
    .setElevation(elevation - eSpace)
    .setCount(count)
    .setColor(color1)
    .setPhase(-hSpace)
    .setValue(2).setLives(2);
  config->addBrickRing()
    .setElevation(elevation - eSpace)
    .setCount(count)
    .setColor(color2)
    .setPhase(hSpace);
  config->addBrickRing()
    .setElevation(elevation + eSpace)
    .setCount(count)
    .setColor(color2)
    .setPhase(-hSpace);
  config->addBrickRing()
    .setElevation(elevation + eSpace)
    .setCount(count)
    .setColor(color1)
    .setPhase(hSpace)
    .setValue(2).setLives(2);
}

RoundConfig* RoundConfig::createRoundConfig3() {
  RoundConfig* config = new RoundConfig("Round3");
  config->_brickSize.set(7.0f, 5.0f, 17.0f);
  config->_paddleSize.set(16.0f, 8.0f, 40.0f);
  config->_ballRadius = 8.0f;
  config->_modifierRadius = 9.0f;
  
  for (int i = 0; i < 5; i ++) {
    config->addBall(BallSpec(30, ofRandom(360)));
  }
  
  createBrickGroups(config, 45, 12, 1.5, 4);
  createBrickGroups(config, 55, 12, 1.5, 4);
  createBrickGroups(config, 65, 3, 1.5, 8);
  createBrickGroups(config, 75, 3, 1.5, 12);
  
  config->addBrickRing()
    .setElevation(30)
    .setCount(24)
    .setColor(ofColor(0, 255, 255));
  
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
  config->addStartMessage("STAGE 3 START", ofColor(0, 255, 0))
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
    .setSpin(ValuePulserSpec<ofVec3f>(0, 0.2, 5.0f, ofVec3f(0)))
    .setSpread(ValuePulserSpec<ofVec3f>(0.01, 0.16, 10.0f, ofVec3f(0)),
               ofVec3f(60))
    .setCount(50)
    .setRadiusScale(2)
    .setLineWidth(0.9)
    .setColor(ofColor(0, 127, 255, 63));
  
  return config;
}
