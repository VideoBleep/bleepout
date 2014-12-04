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
#include "GameObject.h"
#include "GameObjectCollection.h"

class RendererBase;

class RoundController {
public:
  RoundController(RoundConfig config,
                  PlayerManager& playerManager,
                  RendererBase& renderer);
  void setup();
  void draw();
  void update();
  
  const GameObjectCollection<Paddle>& paddles() const { return _paddles; }
  const GameObjectCollection<Ball>& balls() const { return _balls; }
  const GameObjectCollection<Brick>& bricks() const { return _bricks; }
  GameObjectCollection<Paddle>& paddles() { return _paddles; }
  GameObjectCollection<Ball>& balls() { return _balls; }
  GameObjectCollection<Brick>& bricks() { return _bricks; }

private:
  void addBrick(ofVec2f center);
  void addBall(ofVec2f center);
  void addPaddle(ofVec2f center, Player& player);
  
  void contactStart(ofxBox2dContactArgs& e);
  void contactEnd(ofxBox2dContactArgs& e);
  
  void ballHitBrick(Ball& ball, Brick& brick);
  void ballHitPaddle(Ball& ball, Paddle& paddle);
private:
  GameObjectCollection<Paddle> _paddles;
  GameObjectCollection<Ball>   _balls;
  GameObjectCollection<Brick>  _bricks;
  PlayerManager& _playerManager;
  ofxBox2d _box2d;
  RoundConfig _config;
  RendererBase& _renderer;

};

#endif /* defined(__bleepout__RoundController__) */
