//
//  SpaceController.h
//  bleepout
//
//  Created by tekt on 12/7/14.
//
//

#ifndef __bleepout__SpaceController__
#define __bleepout__SpaceController__

#include "GameState.h"
#include "BleepoutConfig.h"
#include "GameEvents.h"
#include "PhysicsWorld.h"

class SpaceController : public CollisionEventSource {
public:
  SpaceController(RoundState& state, RoundConfig& config);
  
  void setup();
  void update();
  void drawDebug();
    
private:
  void generateBricks();
  
  void addBrick(ofVec3f center);
  void addBall(ofVec3f center);
  void addPaddle(ofVec3f center, Player* player);
  
  void onCollision(CollisionArgs &cdata);
    
  void ballHitObject(Ball* ball, GameObject* obj);

private:
  PhysicsWorld _world;
  RoundState& _state;
  RoundConfig& _config;
};

#endif /* defined(__bleepout__SpaceController__) */
