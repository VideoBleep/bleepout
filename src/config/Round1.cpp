//
//  Round1.cpp
//  bleepout
//
//  Created by tekt on 12/21/14.
//
//

#include "BleepoutConfig.h"
#include "BleepoutParameters.h"

RoundConfig* RoundConfig::createRoundConfig1() {
  const auto& appParams = BleepoutParameters::get();
  RoundConfig* config = new RoundConfig("Round1");
  config->_paddleSize.set(16.0f, 8.0f, 40.0f);
  config->_ballRadius = 8.0f;
  config->_modifierSize.set(9.0f, 16.0f, 9.0f);
  config->_rules.setBallsRespawn(true);
  
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
    .setColor(ofColor::red)
    .setBeneficial(false);
  
  int cols = 12;
  int rows = 10;
  for (int col = 0; col < cols; col++) {
    float s = col / (float)cols;
    auto& spec = config->addCurvedBrickColumn()
      .setEnd1(30, s * 360 + ((col % 2) ? 5 : -5))
      .setEnd2(60, s * 360 + 20 + ((col % 2) ? 5 : -5))
      .setCount(rows)
      .setColor(ofColor(s * 255, 0, 255),
                ofColor(s * 255, 255, 0))
      .setSize(ofVec3f(7, 5, 17))
      .setStripe1(CurvedBrickColumnSpec::StripeSpec(1, 1, "", 0))
      .setStripe2(CurvedBrickColumnSpec::StripeSpec(2, 1, widePaddleName, 0.3))
      .setStripe3(CurvedBrickColumnSpec::StripeSpec(1, 1, narrowPaddleName, 0.2));
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
    .setSpeed(0.02)
    .setSize(ofVec3f(7.0f, 5.0f, 17.0f));
  config->addBrickRing()
    .setElevation(76)
    .setColor(ofColor::black)
    .setCount(10)
    .setSpeed(-0.02)
    .setSize(ofVec3f(7.0f, 5.0f, 17.0f));
  config->addBrickRing()
    .setElevation(80)
    .setColor(ofColor::black)
    .setCount(8)
    .setValue(2)
    .setLives(2)
    .setSpeed(0.02)
    .setSize(ofVec3f(7.0f, 5.0f, 17.0f));
  
  // Create the floor exit wall
  float d = (appParams.domeMargin + appParams.domeRadius) * 5;
  config->addWall()
    .setElevation(-10)
    .setHeading(0)
    .setSize(ofVec3f(d, 10, d))
    .setIsExit(true)
    .setVisible(false);
  
  config->addStartMessage("VideoBleep\n presents", ofColor(255))
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
