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
#include "RoundComponent.h"
#include <ofMain.h>

class SpaceController : public EventSource, public RoundComponent {
public:
  SpaceController(RoundState& state,
                  const RoundConfig& config,
                  const BleepoutParameters& appParams);
  
  ofEvent<CollisionEventArgs> collisionEvent;
  
  void setup();
  void update();
  void drawDebug();
    
  void addInitialPaddles();
    
  void addBall(const BallSpec& ballSpec);
  void addPaddle(float heading, Player* player);
  
  void setUpModifier(Modifier& modifier,
                     Brick& spawnerBrick);
  void removeModifier(Modifier& modifier);
    
private:
  void addBrick(const BrickSpec& brickSpec);
  void addWall(const WallSpec& wallSpec);
  
  void onCollision(CollisionArgs &cdata);

  void notifyCollision(GameObject* a, GameObject* b);
  bool shouldBounce(const Ball& ball, const GameObject& object) const;

private:
  PhysicsWorld _world;
};

#endif /* defined(__bleepout__SpaceController__) */
