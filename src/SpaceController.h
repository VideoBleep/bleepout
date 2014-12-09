//
//  SpaceController.h
//  bleepout
//
//  Created by tekt on 12/7/14.
//
//

#ifndef __bleepout__SpaceController__
#define __bleepout__SpaceController__

#include <ofxBox2d.h>

#include "GameState.h"
#include "BleepoutConfig.h"
#include "GameEvents.h"

class SpaceController : public CollisionEventSource {
public:
  SpaceController(RoundState& state, RoundConfig& config);
  
  void setup();
  void update();
private:
  void generateBricks();
  
  void addBrick(ofVec2f center);
  void addBall(ofVec2f center);
  void addPaddle(ofVec2f center, Player* player);
  
  void contactStart(ofxBox2dContactArgs& e);
  void contactEnd(ofxBox2dContactArgs& e);
  
  void ballHitObject(Ball* ball, GameObject* obj);

private:
  ofxBox2d _box2d;
  RoundState& _state;
  RoundConfig& _config;
};

#endif /* defined(__bleepout__SpaceController__) */
