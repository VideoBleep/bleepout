//
//  RoundManager.h
//  bleepout
//
//  Created by tekt on 12/2/14.
//
//

#ifndef __bleepout__RoundController__
#define __bleepout__RoundController__

#include <ofMain.h>
#include <ofxBox2d.h>
#include <vector>
#include "PlayerManager.h"
#include "Paddle.h"
#include "Ball.h"
#include "Brick.h"
#include "BleepoutConfig.h"

class RoundController {
public:
  RoundController(RoundConfig config,
                  PlayerManager& playerManager);
  void setup();
  void draw();
  void update();
private:
  void addBrick(ofVec2f center);
  void addBall(ofVec2f center);
  void addPaddle(ofVec2f center, Player& player);
private:
  std::vector<ofPtr<Paddle> > _paddles;
  std::vector<ofPtr<Ball> >   _balls;
  std::vector<ofPtr<Brick> >  _bricks;
  PlayerManager& _playerManager;
  ofxBox2d _box2d;
  RoundConfig _config;
};

#endif /* defined(__bleepout__RoundController__) */
