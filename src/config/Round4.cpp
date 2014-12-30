//
//  Round4.cpp
//  bleepout
//
//  Created by tekt on 12/26/14.
//
//

#include "BleepoutConfig.h"
#include "BleepoutParameters.h"

RoundConfig* RoundConfig::createRoundConfig4() {
  const auto& appParams = BleepoutParameters::get();
  RoundConfig* config = new RoundConfig("Round4");
  config->_paddleSize.set(16, 8, 40);
  config->_ballRadius = 8;
  config->_modifierSize.set(9, 16, 9);
  
  for (int i = 0; i < 5; i++) {
    config->addBall(BallSpec(30, ofRandom(360)));
  }
  
  std::string superchargedModName("supercharge");
  config->addModifierDef(superchargedModName, MODIFIER_SUPERCHARGED_BALL)
    .setColor(ofColor::green)
    .setDuration(20);
  
  {
    BrickQuadsSpec prototype;
    prototype
      .setColor(ofColor(0, 255, 0), ofColor(0, 127, 127))
      .setSize(ofVec3f(7, 5, 14))
      .setModifier(superchargedModName, .7);
    
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
    .setElevation(85)
    .setCount(24)
    .setSize(ofVec3f(7, 5, 4))
    .setColor(ofColor(0, 63, 255));
  config->addBrickRing()
    .setElevation(90)
    .setCount(24)
    .setSize(ofVec3f(7, 5, 4))
    .setColor(ofColor(0, 63, 255));
  config->addBrickRing()
    .setElevation(80)
    .setCount(24)
    .setSize(ofVec3f(7, 5, 4))
    .setColor(ofColor(0, 63, 255));
  
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
    .setModifier(superchargedModName, .5);
  
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
  
  {
    WallRingSpec prototype;
    prototype.setVisible(true)
      .setIsExit(false)
      .setSpeed(0)
      .setCount(12);
    config->addWallRing()
      .copyFrom(prototype)
      .setElevation(36)
      .setSize(ofVec3f(7, 5, 12));
    config->addWallRing()
      .copyFrom(prototype)
      .setElevation(34)
      .setSize(ofVec3f(7, 5, 10));
    config->addWallRing()
      .copyFrom(prototype)
      .setElevation(32)
      .setSize(ofVec3f(7, 5, 8));
    config->addWallRing()
      .copyFrom(prototype)
      .setElevation(28)
      .setSize(ofVec3f(7, 5, 8));
    config->addWallRing()
      .copyFrom(prototype)
      .setElevation(26)
      .setSize(ofVec3f(7, 5, 8));
    config->addWallRing()
      .copyFrom(prototype)
      .setElevation(22)
      .setSize(ofVec3f(7, 5, 8));
  }
  
  // Create the floor exit wall
  float d = (appParams.domeMargin + appParams.domeRadius) * 5;
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
