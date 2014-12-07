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
#include "GameEvents.h"
#include "GameState.h"

class RendererBase;

class RoundController: public RoundEventSender
{
public:
  RoundController(RoundConfig config,
                  PlayerManager& playerManager,
                  RendererBase& renderer);
  
  ~RoundController();
  
  void setup();
  void draw();
  void update();
  
  const GameObjectCollection<Paddle>& paddles() const { return _state.paddles(); }
  const GameObjectCollection<Ball>& balls() const { return _state.balls(); }
  const GameObjectCollection<Brick>& bricks() const { return _state.bricks(); }
  GameObjectCollection<Paddle>& paddles() { return _state.paddles(); }
  GameObjectCollection<Ball>& balls() { return _state.balls(); }
  GameObjectCollection<Brick>& bricks() { return _state.bricks(); }
  
  void dumpToLog();
  
  void keyPressed(int key);
  void mouseMoved(int x, int y );
  void mouseDragged(int x, int y, int button);
  
  void setPaddlePosition(GameObjectId playerId, float xPercent);

private:
  void generateBricks();
  
  void addBrick(ofVec2f center);
  void addBall(ofVec2f center);
  void addPaddle(ofVec2f center, ofPtr<Player> player);
  
  void contactStart(ofxBox2dContactArgs& e);
  void contactEnd(ofxBox2dContactArgs& e);
  
  void ballHitObject(Ball& ball, GameObject& obj);
  void ballHitBrick(Ball& ball, Brick& brick);
  void ballHitPaddle(Ball& ball, Paddle& paddle);
private:
  PlayerManager& _playerManager;
  ofxBox2d _box2d;
  RoundConfig _config;
  RendererBase& _renderer;
  GameState _state;
};

#endif /* defined(__bleepout__RoundController__) */
