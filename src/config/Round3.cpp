//
//  Round3.cpp
//  bleepout
//
//  Created by tekt on 12/21/14.
//
//

#include "BleepoutConfig.h"

RoundConfig* RoundConfig::createRoundConfig3() {
  RoundConfig* config = new RoundConfig("Round3");
  config->_brickSize.set(7.0f, 5.0f, 17.0f);
  config->_paddleSize.set(16.0f, 8.0f, 40.0f);
  config->_ballRadius = 8.0f;
  config->_modifierRadius = 9.0f;
  
  for (int i = 0; i < 5; i ++) {
    config->addBall(BallSpec(30, ofRandom(360)));
  }
  
  {
    int groups = 12;
    float elevation = 35;
    float eSpace = 1.5;
    float hSpace = 4;
    for (int group = 0; group < groups; group++) {
      float heading = (group / (float)(groups)) * 360.0;
      ofColor color1(0, 255, 0);
      ofColor color2(0, 127, 127);
      config->addBrick()
        .setElevation(elevation + eSpace)
        .setHeading(heading - hSpace)
        .setColor(color1);
      config->addBrick()
        .setElevation(elevation + eSpace)
        .setHeading(heading + hSpace)
        .setColor(color2)
        .setValue(2)
        .setLives(2);
      config->addBrick()
        .setElevation(elevation - eSpace)
        .setHeading(heading - hSpace)
        .setColor(color2)
        .setValue(2)
        .setLives(2);
      config->addBrick()
        .setElevation(elevation - eSpace)
        .setHeading(heading + hSpace)
        .setColor(color1);
    }
  }
  
  config->addWallRing()
    .setElevation(30)
    .setPhase(360 / 24.0)
    .setCount(12)
    .setSize(ofVec3f(8, 3, 25))
    .setVisible(true);
  config->addWallRing()
    .setElevation(33)
    .setPhase(360 / 24.0)
    .setCount(12)
    .setSize(ofVec3f(8, 3, 25))
    .setVisible(true);
  config->addWallRing()
    .setElevation(36)
    .setPhase(360 / 24.0)
    .setCount(12)
    .setSize(ofVec3f(8, 3, 25))
    .setVisible(true);
  
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
