//
//  Round2.cpp
//  bleepout
//
//  Created by tekt on 12/21/14.
//
//

#include "BleepoutConfig.h"
#include "BleepoutParameters.h"

RoundConfig* RoundConfig::createRoundConfig2() {
  const auto& appParams = BleepoutParameters::get();
  RoundConfig* config = new RoundConfig("Round2");
  config->_paddleSize.set(16.0f, 8.0f, 40.0f);
  config->_ballRadius = 8.0f;
  config->_modifierSize.set(9.0f, 16.0f, 9.0f);
  
  for (int i = 0; i < 5; i ++) {
    config->addBall(BallSpec(30, ofRandom(360)));
  }
  //...
  config->addStartMessage("VideoBleep\n presents", ofColor(255))
    .setSize(12)
    .setTiming(0, 3);
  config->addStartMessage("BLEEPOUT", ofColor(0, 120, 240))
    .setSize(50)
    .setTrails(4)
    .setTiming(3, 4.5);
  config->addStartMessage("STAGE 2 START", ofColor(0, 255, 255))
    .setSize(25)
    .setTiming(7.5, 3);
  config->_startDelay = 10;
  
  std::string superchargeModName("supercharge");
  config->addModifierDef(superchargeModName, MODIFIER_SUPERCHARGED_BALL)
    .setColor(ofColor::green)
    .setDuration(20);
  
  {
    int cols = 18;
    int rows = 14;
    for (int col = 0; col < cols; col++) {
      float s = col / (cols * 1.0);
      auto& spec = config->addCurvedBrickColumn()
        .setEnd1(35, s * 360)
        .setEnd2(75, s * 360 + 3 * (rows-1))
        .setCount(rows)
        .setColor(ofColor(0, 255, 255),
                  ofColor(0, 255, 0))
        .setSize(ofVec3f(5, 2, 10))
        .setStripe1(CurvedBrickColumnSpec::StripeSpec(1, 1, "", 0))
        .setStripe2(CurvedBrickColumnSpec::StripeSpec(2, 1, superchargeModName, 0.3))
        .setStripe3(CurvedBrickColumnSpec::StripeSpec(1, 1, "", 0));
    }
  }
  
  // Create the floor exit wall
  float d = (appParams.domeMargin + appParams.domeRadius) * 5;
  config->addWall()
    .setElevation(-10)
    .setHeading(0)
    .setSize(ofVec3f(d, 10, d))
    .setIsExit(true)
    .setVisible(false);
  
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
