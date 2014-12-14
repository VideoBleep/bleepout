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
    
  void addInitialPaddles();
    
private:
  void generateBricks();
  
  void addPaddle(float heading, Player* player);
  
  void addBrick(const BrickSpec& brickSpec);
  void addBrickRing(const BrickRingSpec& ringSpec);
  void addWall(const WallSpec& wallSpec);
  void addCurvedWall(const CurvedWallSpec& curveSpec);
  void addBall(const BallSpec& ballSpec);
  
  void onCollision(CollisionArgs &cdata);
    
  void ballHitObject(Ball* ball, GameObject* obj, ofVec3f normal);

private:
  PhysicsWorld _world;
  RoundState& _state;
  RoundConfig& _config;
};

#endif /* defined(__bleepout__SpaceController__) */
