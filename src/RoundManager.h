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
#include <vector>
#include "PlayerManager.h"
#include "Paddle.h"
#include "Ball.h"
#include "Brick.h"
#include "BleepoutConfig.h"
#include "GameObject.h"
#include "GameObjectCollection.h"
#include "GameEvents.h"
#include "Physics.h"

class RendererBase;

class RoundController: public RoundEventSender
{
public:
  RoundController(RoundConfig config,
                  PlayerManager& playerManager,
                  RendererBase& renderer,
                  Physics& physics);
  
  ~RoundController();
  
  void setup();
  void draw();
  void update();
  
  const GameObjectCollection<Paddle>& paddles() const { return _paddles; }
  const GameObjectCollection<Ball>& balls() const { return _balls; }
  const GameObjectCollection<Brick>& bricks() const { return _bricks; }
  GameObjectCollection<Paddle>& paddles() { return _paddles; }
  GameObjectCollection<Ball>& balls() { return _balls; }
  GameObjectCollection<Brick>& bricks() { return _bricks; }
  
  void dumpToLog();
  
  void keyPressed(int key);
  void mouseMoved(int x, int y );
  void mouseDragged(int x, int y, int button);
  
  void setPaddlePosition(GameObjectId playerId, float xPercent);

private:
  void generateBricks();
  
  void addBrick(ofVec3f center);
  void addBall(ofVec3f center);
  void addPaddle(ofVec3f center, ofPtr<Player> player);
  
  void collisionDetected(CollisionArgs& e);
  
  void ballHitObject(Ball& ball, GameObject& obj);
  void ballHitBrick(Ball& ball, Brick& brick);
  void ballHitPaddle(Ball& ball, Paddle& paddle);
private:
  PlayerManager& _playerManager;
  RoundConfig _config;
  RendererBase& _renderer;
  Physics _physics;
  
  GameObjectCollection<Paddle> _paddles;
  GameObjectCollection<Ball>   _balls;
  GameObjectCollection<Brick>  _bricks;

};

#endif /* defined(__bleepout__RoundController__) */
