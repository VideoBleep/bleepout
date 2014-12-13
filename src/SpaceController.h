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
#include <ofMain.h>

class SpaceController : public CollisionEventSource {
public:
  SpaceController(RoundState& state, RoundConfig& config);
  
  void setup();
  void update();
  void drawDebug();
    
private:
  void generateBricks();
  
  void addBrick(float elevation, float heading, const ofColor& color);
  void addBall(float elevation, float heading);
  void addPaddle(float heading, Player* player);
  void addWall(float elevation, float heading, float width, float height, float depth);
    
  void addBrickRing(float elevation, const ofColor& color, int count, float phase = 0);
  void addCurvedWall(float elevation1, float heading1, float elevation2, float heading2, float width);
  
  void onCollision(CollisionArgs &cdata);
    
  void ballHitObject(Ball* ball, GameObject* obj, ofVec3f normal);

private:
  PhysicsWorld _world;
  RoundState& _state;
  RoundConfig& _config;
};

#endif /* defined(__bleepout__SpaceController__) */
