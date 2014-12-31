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
  SpaceController(RoundState& state);
  
  ofEvent<CollisionEventArgs> collisionEvent;
  
  void setup();
  void update();
  void drawDebug();
  
  void addInitialPaddles();
  void loadBricksAndWalls();
  void addInitialBalls();
  
  void resetState();
  
  Ball& addBall(const BallSpec& ballSpec);
  void addPaddle(float heading, Player* player);
  
  void setUpModifier(Modifier& modifier,
                     Brick& spawnerBrick);
  void removeObject(PhysicsObject& object);
  
  template<typename Seq>
  void removeObjects(Seq& objects) {
    for (auto& obj : objects) {
      removeObject(*obj);
    }
  }
  
  const char* eventSourceName() const override { return "SpaceController"; }

private:
  void addBrick(const BrickSpec& brickSpec);
  void addWall(const WallSpec& wallSpec);

  void onCollision(CollisionArgs &cdata);

  void notifyCollision(GameObject* a, GameObject* b);

private:
  PhysicsWorld _world;
  float _lastUpdateTime;
};

#endif /* defined(__bleepout__SpaceController__) */
