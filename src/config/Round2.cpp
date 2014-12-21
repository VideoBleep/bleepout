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
  config->addStartMessage("STAGE 2 START", ofColor(0, 255, 0))
    .setSize(25)
    .setTiming(7.5, 3);
  
  int cols = 14;
  int rows = 10;
  
  for (int col = 0; col < cols; col++) {
    for (int row = 0; row < rows; row++) {
      float s = col / (cols * 1.0);
      config->addBrick()
        .setElevation(30 + 3 * row)
        .setHeading(s * 360 + row * 6)
        .setColor((row % 3 == 1) ? ofColor::green : ofColor::brown)
        .setLives((row % 3 == 1) ? 2 : 1)
        .setValue(1)
        .setSpeed(0);
    }
  }
  return config;
}
